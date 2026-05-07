#include "level3.h"
#include "mazetower.h"
#include <QPainter>
#include <QFont>
#include <QApplication>
#include <QRandomGenerator>
#include <QtMath>
#include <QTimer>

Level3Window::Level3Window(QWidget *parent)
    : QWidget(parent),
    playerX(120), playerY(120), playerHp(30), playerAlive(true),
    keyW(false), keyA(false), keyS(false), keyD(false),
    aimAngle(0),
    spawnCooldownTimer(0), attackCooldown(0),
    collapseSpawnTimer(0),
    gameRunning(true), gameTime(0)
{
    setFixedSize(1600, 900);
    setWindowTitle("迷宫塔 - 关卡3");
    setMouseTracking(true);
    initMaze();
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Level3Window::gameLoop);
    timer->start(16);
}


void Level3Window::initMaze(){
    walls.clear();
    // 外边框
    walls.append(QRectF(100, 100, 1400, 20));
    walls.append(QRectF(100, 800, 1400, 20));
    walls.append(QRectF(100, 100, 20, 700));
    walls.append(QRectF(1480, 100, 20, 700));
    //墙体
    walls.append(QRectF(100, 170, 140, 20));
    walls.append(QRectF(360, 170, 20, 70));
    walls.append(QRectF(220, 240, 280, 20));
    walls.append(QRectF(500, 100, 20, 210));
    walls.append(QRectF(640, 170, 20, 70));
    walls.append(QRectF(640, 170, 280, 20));
    walls.append(QRectF(920, 170, 20, 70));
    walls.append(QRectF(920, 240, 140, 20));
    walls.append(QRectF(1060, 170, 20, 210));
    walls.append(QRectF(1060, 170, 140, 20));
    walls.append(QRectF(1340, 170, 20, 70));
    walls.append(QRectF(1340, 240, 140, 20));
    walls.append(QRectF(100, 310, 140, 20));
    walls.append(QRectF(500, 310, 420, 20));
    walls.append(QRectF(780, 240, 20, 350));
    walls.append(QRectF(360, 310, 20, 70));
    walls.append(QRectF(1200,240, 20, 70));
    walls.append(QRectF(1200, 310, 140, 20));
    walls.append(QRectF(1340, 310, 20, 70));
    walls.append(QRectF(1340, 380, 140, 20));
    walls.append(QRectF(220, 380, 280, 20));
    walls.append(QRectF(220, 380, 20, 70));
    walls.append(QRectF(500, 380, 20, 70));
    walls.append(QRectF(920, 380, 280, 20));
    walls.append(QRectF(920, 380, 20, 140));
    walls.append(QRectF(1200, 380, 20, 70));
    walls.append(QRectF(640, 380, 20, 140));
    walls.append(QRectF(100,450, 140, 20));
    walls.append(QRectF(1060, 450, 280, 20));
    walls.append(QRectF(360, 450, 20, 70));
    walls.append(QRectF(1060, 450, 20, 140));
    walls.append(QRectF(1340, 450, 20, 70));
    walls.append(QRectF(220, 520, 160, 20));
    walls.append(QRectF(500, 520, 280, 20));
    walls.append(QRectF(1200, 520, 20, 140));
    walls.append(QRectF(220, 520, 20, 70));
    walls.append(QRectF(500, 520, 20, 70));
    walls.append(QRectF(220, 590, 300, 20));
    walls.append(QRectF(920, 590, 160, 20));
    walls.append(QRectF(1340, 590, 140, 20));
    walls.append(QRectF(360, 590, 20, 140));
    walls.append(QRectF(640, 590, 20, 70));
    walls.append(QRectF(920, 590, 20, 70));
    walls.append(QRectF(100, 660, 140, 20));
    walls.append(QRectF(500, 660, 440, 20));
    walls.append(QRectF(1060, 660, 280, 20));
    walls.append(QRectF(780, 660, 20, 140));
    walls.append(QRectF(220, 730, 280, 20));
    walls.append(QRectF(640, 730, 20, 70));
    walls.append(QRectF(920, 730, 160, 20));
    walls.append(QRectF(1060, 660, 20, 70));
    walls.append(QRectF(1200, 730, 20, 70));
    walls.append(QRectF(1340, 730, 140, 20));
    walls.append(QRectF(1340, 660, 20, 70));





    // 出口位置
    exitRect = QRectF(1340, 760, 60, 60);
    playerX = 120; playerY = 120;
}



bool Level3Window::isWall(qreal x, qreal y, qreal w, qreal h) const {
    if (x < 120 || x + w > 1480 || y < 120 || y + h > 800) return true;
    QRectF r(x, y, w, h);
    for (const auto& wall : walls)
        if (r.intersects(wall)) return true;
    return false;
}

void Level3Window::clampPlayer() {
    qreal mi = 120, ma = 1480 - PLAYER_SIZE;
    if (playerX < mi) playerX = mi;
    if (playerX > ma) playerX = ma;
    mi = 120; ma = 800 - PLAYER_SIZE;
    if (playerY < mi) playerY = mi;
    if (playerY > ma) playerY = ma;
}

void Level3Window::trySpawnEnemy() {
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
            if (!isWall(ex - Enemy::SIZE / 2, ey - Enemy::SIZE / 2, Enemy::SIZE, Enemy::SIZE)) {
                ok = true;
                break;
            }
        }
        if (ok) enemies.append(new Enemy(ex, ey));
    }

    spawnCooldownTimer = 150 + QRandomGenerator::global()->bounded(500);
}

void Level3Window::spawnCollapseZone() {
    collapseSpawnTimer++;
    if (collapseSpawnTimer < 600) return;
    collapseSpawnTimer = 0;

    qreal cx, cy;
    bool ok = false;
    for (int att = 0; att < 50; att++) {
        qreal ang = QRandomGenerator::global()->generateDouble() * 2 * M_PI;
        qreal dis = 100 + QRandomGenerator::global()->generateDouble() * 150;
        cx = playerX + qCos(ang) * dis;
        cy = playerY + qSin(ang) * dis;
        cx = qRound(cx / 32) * 32;
        cy = qRound(cy / 32) * 32;
        if (!isWall(cx, cy, CollapseZone::SIZE, CollapseZone::SIZE)) {
            ok = true;
            break;
        }
    }
    if (ok) collapseZones.append(new CollapseZone(cx, cy));
}

void Level3Window::gameLoop() {
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

void Level3Window::checkCollisions() {
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

void Level3Window::gameOver(bool win) {
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

void Level3Window::paintEvent(QPaintEvent *) {
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
        p.setBrush(QColor(139, 90, 43));
        p.drawRoundedRect(playerX, playerY, PLAYER_SIZE, PLAYER_SIZE, 4, 4);

        qreal cx = playerX + PLAYER_SIZE / 2;
        qreal cy = playerY + PLAYER_SIZE / 2;
        qreal dx = qCos(aimAngle);
        qreal dy = qSin(aimAngle);
        p.setPen(QPen(QColor(255, 220, 0), 5));
        p.drawLine(cx, cy, cx + dx * 30, cy + dy * 30);
        p.setBrush(QColor(255, 200, 0));
        p.drawEllipse(cx + dx * 30 - 4, cy + dy * 30 - 4, 8, 8);
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

void Level3Window::drawMaze(QPainter &p) {
    for (const auto& w : walls) {
        p.setBrush(QColor(100, 85, 70));
        p.drawRect(w);
    }
}

void Level3Window::drawHUD(QPainter &p) {
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

void Level3Window::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
    case Qt::Key_W: case Qt::Key_Up: keyW = true; break;
    case Qt::Key_S: case Qt::Key_Down: keyS = true; break;
    case Qt::Key_A: case Qt::Key_Left: keyA = true; break;
    case Qt::Key_D: case Qt::Key_Right: keyD = true; break;
    case Qt::Key_Escape: close(); break;
    }
}

void Level3Window::keyReleaseEvent(QKeyEvent *e) {
    switch (e->key()) {
    case Qt::Key_W: case Qt::Key_Up: keyW = false; break;
    case Qt::Key_S: case Qt::Key_Down: keyS = false; break;
    case Qt::Key_A: case Qt::Key_Left: keyA = false; break;
    case Qt::Key_D: case Qt::Key_Right: keyD = false; break;
    }
}

void Level3Window::mouseMoveEvent(QMouseEvent *e) {
    qreal cx = playerX + PLAYER_SIZE / 2;
    qreal cy = playerY + PLAYER_SIZE / 2;
    aimAngle = qAtan2(e->y() - cy, e->x() - cx);
}

void Level3Window::mousePressEvent(QMouseEvent *) {
    if (!gameRunning || attackCooldown > 0) return;
    qreal cx = playerX + PLAYER_SIZE / 2;
    qreal cy = playerY + PLAYER_SIZE / 2;
    qreal dx = qCos(aimAngle) * Projectile::SPEED;
    qreal dy = qSin(aimAngle) * Projectile::SPEED;
    projectiles.append(new Projectile(cx, cy, dx, dy));
    attackCooldown = 15;
}