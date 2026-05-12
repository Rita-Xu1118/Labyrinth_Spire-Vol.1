#include "level2.h"
#include "mazetower.h"
#include <QPainter>
#include <QFont>
#include <QMessageBox>
#include <QApplication>
#include <QRandomGenerator>
#include <QtMath>
#include <QTimer>
#include"audiomanager.h"

Level2Window::Level2Window(QWidget *parent)
    : QWidget(parent),
    playerX(120), playerY(120), playerHp(30), playerAlive(true),
    keyW(false), keyA(false), keyS(false), keyD(false),
    aimAngle(0),
    spawnCooldownTimer(0), attackCooldown(0),
    collapseSpawnTimer(0),
    gameRunning(true), gameTime(0)
{
    setFixedSize(1600, 900);
    setWindowTitle("迷宫塔 - 关卡2");
    setMouseTracking(true);
    initMaze();
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Level2Window::gameLoop);
    timer->start(16);
    AudioManager::instance()->play("qrc:/level2bgm.mp3");
}

void Level2Window::initMaze(){
    {
        // 外边框
        walls.append(QRectF(100, 100, 1400, 20));   // 上边框
        walls.append(QRectF(100, 800, 1400, 20));   // 下边框
        walls.append(QRectF(100, 100, 20, 700));    // 左边框
        walls.append(QRectF(1480, 100, 20, 700));   // 右边框
        //墙体
        walls.append(QRectF(780,100,20,140));
        walls.append(QRectF(220,170,140,20));
        walls.append(QRectF(500,170,160,20));
        walls.append(QRectF(920,170,300,20));
        walls.append(QRectF(220,170,20,140));
        walls.append(QRectF(500,170,20,70));
        walls.append(QRectF(1060,170,20,140));
        walls.append(QRectF(1340,170,20,140));
        walls.append(QRectF(360,240,560,20));
        walls.append(QRectF(1060,240,140,20));
        walls.append(QRectF(1340,310,140,20));
        walls.append(QRectF(920,240,20,70));
        walls.append(QRectF(100,310,140,20));
        walls.append(QRectF(640,240,20,210));
        walls.append(QRectF(780,310,160,20));
        walls.append(QRectF(1200,240,20,350));
        walls.append(QRectF(360,310,20,90));
        walls.append(QRectF(500,310,20,140));
        walls.append(QRectF(220,380,140,20));
        walls.append(QRectF(220,380,20,70));
        walls.append(QRectF(100,450,140,20));
        walls.append(QRectF(360,450,560,20));
        walls.append(QRectF(780,380,420,20));
        walls.append(QRectF(1060,380,20,90));
        walls.append(QRectF(1340,380,20,70));
        walls.append(QRectF(1340,450,140,20));
        walls.append(QRectF(220,520,280,20));
        walls.append(QRectF(640,520,560,20));
        walls.append(QRectF(500,520,20,210));
        walls.append(QRectF(780,520,20,70));
        walls.append(QRectF(500,590,300,20));
        walls.append(QRectF(100,590,140,20));
        walls.append(QRectF(1200,590,140,20));
        walls.append(QRectF(1340,520,20,90));
        walls.append(QRectF(360,590,20,210));
        walls.append(QRectF(220,660,20,70));
        walls.append(QRectF(220,730,140,20));
        walls.append(QRectF(500,730,140,20));
        walls.append(QRectF(640,730,20,70));
        walls.append(QRectF(640,660,140,20));
        walls.append(QRectF(780,660,20,70));
        walls.append(QRectF(780,730,160,20));
        walls.append(QRectF(920,590,20,140));
        walls.append(QRectF(920,590,140,20));
        walls.append(QRectF(920,660,560,20));
        walls.append(QRectF(1200,660,20,70));
        walls.append(QRectF(1060,730,20,70));




        exitRect = QRectF(1380, 720, 60, 60); // 右下角出口
        playerX = 130; playerY = 130;



    }
}

bool Level2Window::isWall(qreal x, qreal y, qreal w, qreal h) const {
    QRectF r(x, y, w, h);
    for (const auto& wall : walls) {
        // 缩小墙体一点点，留出缝隙
        QRectF shrunk=wall;
        if (r.intersects(shrunk)) return true;
    }
    return false;
}

void Level2Window::clampPlayer() {
    qreal mi = 50, ma = 1480 - PLAYER_SIZE;
    if (playerX < mi) playerX = mi;
    if (playerX > ma) playerX = ma;
    mi = 120; ma = 800 - PLAYER_SIZE;
    if (playerY < mi) playerY = mi;
    if (playerY > ma) playerY = ma;
}

void Level2Window::trySpawnEnemy() {
    if (gameTime < 120) return;
    if (spawnCooldownTimer > 0) {
        spawnCooldownTimer--;
        return;
    }

    int count = QRandomGenerator::global()->bounded(2) + 1;
    for (int i = 0; i < count; i++) {
        qreal ex, ey;
        bool ok = false;
        for (int att = 0; att < 50; att++) {
            qreal ang = QRandomGenerator::global()->generateDouble() * 2 * M_PI;
            qreal dis = 150 + QRandomGenerator::global()->generateDouble() * 100;
            ex = playerX + qCos(ang) * dis;
            ey = playerY + qSin(ang) * dis;
            if (ex >= 120 && ex <= 1460 && ey >= 120 && ey <= 760
                    && !isWall(ex-Enemy::SIZE/2, ey-Enemy::SIZE/2, Enemy::SIZE, Enemy::SIZE)) {
                ok = true;
                break;
            }
        }
        if (ok) enemies.append(new Enemy(ex, ey));
    }

    spawnCooldownTimer = 250 + QRandomGenerator::global()->bounded(500);
}

void Level2Window::spawnCollapseZone() {
    collapseSpawnTimer++;
    if (collapseSpawnTimer < 600) return;
    collapseSpawnTimer = 0;

    qreal cx, cy;
    bool ok = false;

    // 迷宫有效区域范围
    const qreal MAZE_MIN_X = 120;
    const qreal MAZE_MAX_X = 1460 - CollapseZone::SIZE;
    const qreal MAZE_MIN_Y = 120;
    const qreal MAZE_MAX_Y = 760 - CollapseZone::SIZE;

    for (int att=0;att<50;att++) {
        qreal ang = QRandomGenerator::global()->generateDouble()*2*M_PI;
        qreal dis = 100 + QRandomGenerator::global()->generateDouble()*150;
        cx = playerX + qCos(ang)*dis;
        cy = playerY + qSin(ang)*dis;

        // 对齐到32网格
        cx = qRound(cx/32)*32;
        cy = qRound(cy/32)*32;


        if (cx >= MAZE_MIN_X && cx <= MAZE_MAX_X &&
            cy >= MAZE_MIN_Y && cy <= MAZE_MAX_Y &&
            !isWall(cx, cy, CollapseZone::SIZE, CollapseZone::SIZE))
        {
            ok = true;
            break;
        }
    }
    if (ok) collapseZones.append(new CollapseZone(cx,cy));
}

void Level2Window::gameLoop() {
    if (!gameRunning || !playerAlive) return;
    gameTime++;

    qreal nx = playerX, ny = playerY;
    if (keyW) ny -= PLAYER_SPEED;
    if (keyS) ny += PLAYER_SPEED;
    if (keyA) nx -= PLAYER_SPEED;
    if (keyD) nx += PLAYER_SPEED;

    if (!isWall(nx, playerY, PLAYER_SIZE, PLAYER_SIZE)) playerX = nx;
    if (!isWall(playerX, ny, PLAYER_SIZE, PLAYER_SIZE)) playerY = ny;
    clampPlayer();

    trySpawnEnemy();
    spawnCollapseZone();

    for (auto e : enemies) e->update(playerX + PLAYER_SIZE / 2, playerY + PLAYER_SIZE / 2);
    for (auto p : projectiles) p->update();

    for (int i = projectiles.size() - 1; i >= 0; i--)
        if (projectiles[i]->isOutOfBounds())
        {
            delete projectiles[i];
            projectiles.removeAt(i);
        }

    for (auto z : collapseZones) z->update();
    if (attackCooldown > 0) attackCooldown--;

    for (int i = enemies.size() - 1; i >= 0; i--)
        if (enemies[i]->isDead())
        {
            delete enemies[i];
            enemies.removeAt(i);
        }

    checkCollisions();
    update();
}

void Level2Window::checkCollisions() {
    QRectF pr(playerX, playerY, PLAYER_SIZE, PLAYER_SIZE);
    for (auto e : enemies) {
        if (e->spawning && pr.intersects(e->getRect())) {
            playerHp -= 5;
            e->spawning = false;
            e->visible = true;
        }
        else if (e->isFullyVisible() && !e->isDead() && pr.intersects(e->getRect())) {
            if (e->canAttack()) {
                playerHp -= 2;
                e->resetAttackCooldown();
            }
        }
    }

    for (int i = projectiles.size() - 1; i >= 0; i--) {
        for (int j = enemies.size() - 1; j >= 0; j--) {
            if (enemies[j]->isFullyVisible() && !enemies[j]->isDead()) {
                if (projectiles[i]->getRect().intersects(enemies[j]->getRect())) {
                    enemies[j]->takeDamage(1);
                    delete projectiles[i];
                    projectiles.removeAt(i);
                    break;
                }
            }
        }
    }

    for (auto z : collapseZones) {
        if (z->isCollapsed() && z->getRect().contains(pr)) {
            gameOver(false);
            return;
        }
    }

    if (pr.intersects(exitRect)) {
        gameOver(true);

        return;
    }
    if (playerHp <= 0) {
        playerHp = 0;
        playerAlive = false;
        gameOver(false);
    }
}

void Level2Window::gameOver(bool win) {
    gameRunning = false;
    timer->stop();
    if (win) {
        m_isExitReached = true;
    }
    else {
        m_isFailed = true;
    }
    update();

    QTimer::singleShot(1500, [=]() {
        AudioManager::instance()->stop();
        AudioManager::instance()->play("qrc:/bgm.mp3");
        MazeTower *existingMazeTower = nullptr;
        for (QWidget *w : QApplication::topLevelWidgets()) {
            if (MazeTower *mt = qobject_cast<MazeTower*>(w)) {
                existingMazeTower = mt;
                break;
            }
        }
        if (existingMazeTower) {
            existingMazeTower->show();
            existingMazeTower->raise();
            existingMazeTower->activateWindow();
        }
        else {
            MazeTower *mazeTower = new MazeTower();
            mazeTower->show();
        }
        this->close();
    });
}

void Level2Window::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(rect(), QColor(45, 40, 35));
    drawMaze(p);

    p.setBrush(QColor(100, 150, 100));
    p.drawRect(110, 110, 60, 60);
    p.drawText(110, 110, 60, 60, Qt::AlignCenter, "START");

    p.setBrush(QColor(200, 180, 100));
    p.drawRect(exitRect);
    p.drawText(exitRect, Qt::AlignCenter, "EXIT");

    for (auto z : collapseZones) z->draw(p);
    for (auto e : enemies) e->draw(p);

    if (playerAlive) {
        // 画玩家图片
        QPixmap characterImg("://character.png");
        if (!characterImg.isNull()) {
            qreal displayX = playerX + PLAYER_SIZE/2 - DISPLAY_SIZE/2;
            qreal displayY = playerY + PLAYER_SIZE/2 - DISPLAY_SIZE/2;
            p.drawPixmap(displayX, displayY, DISPLAY_SIZE, DISPLAY_SIZE, characterImg);
        } else {
            p.setBrush(QColor(139, 90, 43));
            p.drawRoundedRect(playerX, playerY, PLAYER_SIZE, PLAYER_SIZE, 4, 4);
        }

        // 三角箭头
        qreal cx = playerX + PLAYER_SIZE/2+30;
        qreal cy = playerY + PLAYER_SIZE/2-15;
        qreal radius = DISPLAY_SIZE/2 + 5;
        qreal arrowLen = 20;
        qreal arrowW = 10;

        qreal tipX = cx + qCos(aimAngle) * radius;
        qreal tipY = cy + qSin(aimAngle) * radius;

        QPolygonF arrow;
        arrow << QPointF(tipX, tipY)
              << QPointF(tipX - qCos(aimAngle + 1.0) * arrowLen,
                         tipY - qSin(aimAngle + 1.0) * arrowLen)
              << QPointF(tipX - qCos(aimAngle - 1.0) * arrowLen,
                         tipY - qSin(aimAngle - 1.0) * arrowLen);

        p.setBrush(QColor(173, 216, 230));
        p.setPen(Qt::NoPen);
        p.drawPolygon(arrow);
    }

    for (auto pr : projectiles) pr->draw(p);
    drawHUD(p);

    if (m_isExitReached) {
        p.fillRect(rect(), QColor(0, 0, 0, 180));
        p.setPen(QColor(255, 255, 255));
        p.setFont(QFont("Arial", 32, QFont::Bold));
        p.drawText(rect(), Qt::AlignCenter, "Congratulations！\nYou'd passed this level！");
    }
    else if (m_isFailed) {
        p.fillRect(rect(), QColor(0, 0, 0, 180));
        p.setPen(QColor(255, 255, 255));
        p.setFont(QFont("Arial", 32, QFont::Bold));
        p.drawText(rect(), Qt::AlignCenter, "Unfortunately！\nYou'd been devoured by the abyes...");
    }
}

void Level2Window::drawMaze(QPainter &p) {
    for (const auto& w : walls) {
        p.setBrush(QColor(100, 85, 70));
        p.drawRect(w);
    }
}

void Level2Window::drawHUD(QPainter &p) {
    p.setBrush(QColor(30, 25, 20, 220));
    p.drawRoundedRect(10, 10, 240, 50, 8, 8);
    p.setPen(Qt::white);
    p.drawText(20, 35, "生命:");
    p.setBrush(QColor(60, 30, 30));
    p.drawRoundedRect(75, 18, 140, 18, 4, 4);
    int w = (playerHp / 30.0) * 140;
    p.setBrush(QColor(200, 60, 60));
    p.drawRoundedRect(75, 18, w, 18, 4, 4);
    p.drawText(225, 35, QString("%1/30").arg(playerHp));
}

void Level2Window::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
    case Qt::Key_W: case Qt::Key_Up: keyW = true; break;
    case Qt::Key_S: case Qt::Key_Down: keyS = true; break;
    case Qt::Key_A: case Qt::Key_Left: keyA = true; break;
    case Qt::Key_D: case Qt::Key_Right: keyD = true; break;
    case Qt::Key_Escape:
        AudioManager::instance()->stop();
        AudioManager::instance()->play("qrc:/bgm.mp3");
        close();
        break;
    }
}

void Level2Window::keyReleaseEvent(QKeyEvent *e) {
    switch (e->key()) {
    case Qt::Key_W: case Qt::Key_Up: keyW = false; break;
    case Qt::Key_S: case Qt::Key_Down: keyS = false; break;
    case Qt::Key_A: case Qt::Key_Left: keyA = false; break;
    case Qt::Key_D: case Qt::Key_Right: keyD = false; break;
    }
}

void Level2Window::mouseMoveEvent(QMouseEvent *e) {
    qreal cx = playerX + PLAYER_SIZE/2;
    qreal cy = playerY + PLAYER_SIZE/2;
    aimAngle = qAtan2(e->y() - cy, e->x() - cx);
}
void Level2Window::mousePressEvent(QMouseEvent *) {
    if (!gameRunning || attackCooldown > 0) return;

    qreal cx = playerX + PLAYER_SIZE/2 +30;
    qreal cy = playerY + PLAYER_SIZE/2-15;

    qreal dx = qCos(aimAngle) * Projectile::SPEED;
    qreal dy = qSin(aimAngle) * Projectile::SPEED;
    projectiles.append(new Projectile(cx, cy, dx, dy));
    attackCooldown = 15;
}