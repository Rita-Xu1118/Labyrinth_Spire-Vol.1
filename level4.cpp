#include "level4.h"
#include "mazetower.h"
#include <QPainter>
#include <QFont>
#include <QApplication>
#include <QRandomGenerator>
#include <QtMath>
#include <QTimer>
#include"audiomanager.h"
Level4Window::Level4Window(QWidget *parent)
    : QWidget(parent),
    playerX(120), playerY(120), playerHp(30), playerAlive(true),
    keyW(false), keyA(false), keyS(false), keyD(false),
    aimAngle(0),
    spawnCooldownTimer(0), attackCooldown(0),
    collapseSpawnTimer(0),
    gameRunning(true), gameTime(0),
    endingState(Normal),
    endingTimer(nullptr),
    textAlpha(255),
    bgAlpha(0),
    phaseTimer(0)
{
    setFixedSize(1600, 900);
    setWindowTitle("迷宫塔 - 关卡4");
    setMouseTracking(true);
    initMaze();
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Level4Window::gameLoop);
    timer->start(16);
     AudioManager::instance()->play("qrc:/level4bgm.mp3");
}

void Level4Window::initMaze(){
    walls.clear();
    walls.append(QRectF(100, 100, 1400, 20));
    walls.append(QRectF(100, 800, 1400, 20));
    walls.append(QRectF(100, 100, 20, 700));
    walls.append(QRectF(1480, 100, 20, 700));
    walls.append(QRectF(100, 170, 140, 20));
    walls.append(QRectF(360, 170, 300, 20));
    walls.append(QRectF(640, 100, 20, 70));
    walls.append(QRectF(780, 100, 20, 140));
    walls.append(QRectF(920, 170, 280, 20));
    walls.append(QRectF(1340, 170, 140, 20));
    walls.append(QRectF(500, 170, 20, 70));
    walls.append(QRectF(1200, 170, 20, 140));
    walls.append(QRectF(220, 240, 140, 20));
    walls.append(QRectF(780, 240, 140, 20));
    walls.append(QRectF(1200, 240, 140, 20));
    walls.append(QRectF(360, 240, 20, 70));
    walls.append(QRectF(640, 240, 20, 280));
    walls.append(QRectF(920, 240, 20, 70));
    walls.append(QRectF(1060, 240, 20, 280));
    walls.append(QRectF(100, 310, 280, 20));
    walls.append(QRectF(500,310, 140, 20));
    walls.append(QRectF(780, 310, 160, 20));
    walls.append(QRectF(1060, 310, 160, 20));
    walls.append(QRectF(1340, 310, 20, 140));
    walls.append(QRectF(1200, 380, 140, 20));
    walls.append(QRectF(220, 380, 280, 20));
    walls.append(QRectF(640, 380, 280, 20));
    walls.append(QRectF(220, 380, 20, 70));
    walls.append(QRectF(500, 380, 20, 70));
    walls.append(QRectF(920, 380, 20, 70));
    walls.append(QRectF(100, 450, 140, 20));
    walls.append(QRectF(500, 450, 140, 20));
    walls.append(QRectF(920, 450, 140, 20));
    walls.append(QRectF(1340, 450, 140, 20));
    walls.append(QRectF(360, 450, 20, 140));
    walls.append(QRectF(780, 450, 20, 140));
    walls.append(QRectF(780, 520, 140, 20));
    walls.append(QRectF(1060, 520, 160, 20));
    walls.append(QRectF(1200, 450, 20, 70));
    walls.append(QRectF(220, 520, 20, 140));
    walls.append(QRectF(360, 590, 980, 20));
    walls.append(QRectF(500, 520, 20, 210));
    walls.append(QRectF(1340, 520, 20, 210));
    walls.append(QRectF(100, 660, 280, 20));
    walls.append(QRectF(640, 660, 280, 20));
    walls.append(QRectF(1060, 590, 20, 70));
    walls.append(QRectF(920, 660, 20, 70));
    walls.append(QRectF(360, 730, 280, 20));
    walls.append(QRectF(780, 730, 20, 70));
    walls.append(QRectF(220, 730, 20, 70));
    walls.append(QRectF(1200, 660, 20, 140));
    walls.append(QRectF(920, 730, 140, 20));
    walls.append(QRectF(1340, 730, 140, 20));
    walls.append(QRectF(1060, 730, 20, 70));

    exitRect = QRectF(1380, 760, 60, 60);
    playerX = 130; playerY = 130;
}

bool Level4Window::isWall(qreal x, qreal y, qreal w, qreal h) const {

    QRectF r(x, y, w, h);
    for (const auto& wall : walls)
        if (r.intersects(wall)) return true;
    return false;
}

void Level4Window::clampPlayer() {
    qreal mi = 50, ma = 1480 - PLAYER_SIZE;
    if (playerX < mi) playerX = mi;
    if (playerX > ma) playerX = ma;
    mi = 120; ma = 800 - PLAYER_SIZE;
    if (playerY < mi) playerY = mi;
    if (playerY > ma) playerY = ma;
}

void Level4Window::trySpawnEnemy() {
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

    spawnCooldownTimer = 150 + QRandomGenerator::global()->bounded(500);
}

void Level4Window::spawnCollapseZone() {
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

void Level4Window::gameLoop() {
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

void Level4Window::checkCollisions() {
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

void Level4Window::gameOver(bool win) {
    gameRunning = false;
    timer->stop();

    if (win) {
        m_isExitReached = true;
        endingState = VictoryText;

        endingTimer = new QTimer(this);
        connect(endingTimer, &QTimer::timeout, this, &Level4Window::nextEndingPhase);
        endingTimer->start(50);

        phaseTimer = 0;
        textAlpha = 255;
        bgAlpha = 0;
    } else {
        m_isFailed = true;
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
            } else {
                MazeTower *mazeTower = new MazeTower();
                mazeTower->show();
            }
            this->close();
        });
    }
    update();
}

void Level4Window::nextEndingPhase() {
    phaseTimer += 50;

    switch (endingState) {
    case VictoryText:
        if (phaseTimer >= 2000) {
            endingState = FadeToBlack;
            phaseTimer = 0;
        }
        break;

    case FadeToBlack:
        bgAlpha = qMin(255, bgAlpha + 5);
        if (bgAlpha >= 255) {
            endingState = Text1_Show;
            phaseTimer = 0;
            textAlpha = 0;
            currentText = "梅芮成功净化了这座迷宫塔";
        }
        break;

    case Text1_Show:
        textAlpha = qMin(255, textAlpha + 15);
        if (textAlpha >= 255 && phaseTimer >= 3000) {
            endingState = Text1_FadeOut;
        }
        break;

    case Text1_FadeOut:
        textAlpha = qMax(0, textAlpha - 15);
        if (textAlpha <= 0) {
            endingState = Text2_Show;
            phaseTimer = 0;
            currentText = "然而，梅芮的远行并没有就此结束";
        }
        break;

    case Text2_Show:
        textAlpha = qMin(255, textAlpha + 15);
        if (textAlpha >= 255 && phaseTimer >= 3000) {
            endingState = Text2_FadeOut;
        }
        break;

    case Text2_FadeOut:
        textAlpha = qMax(0, textAlpha - 15);
        if (textAlpha <= 0) {
            endingState = Text3_Show;
            phaseTimer = 0;
            currentText = "在暂时稳住局势之后，她依旧没有停下脚步\n"
                          "她将要去往更远的异国疆土\n"
                          "寻找散落在大陆各个角落\n"
                          "和自己拥有相同宿命的其他女巫";
        }
        break;

    case Text3_Show:
        textAlpha = qMin(255, textAlpha + 15);
        if (textAlpha >= 255 && phaseTimer >= 8000) {
            endingState = Text3_FadeOut;
        }
        break;

    case Text3_FadeOut:
        textAlpha = qMax(0, textAlpha - 15);
        if (textAlpha <= 0) {
            endingState = Text4_Show;
            phaseTimer = 0;
            currentText = "她们会相聚在一起，一同探寻黑暗力量最初诞生的根源\n"
                          "一起商议能够彻底消灭祸乱的方法\n"
                          "也会一同思索，如何引导世间之人认清本心\n"
                          "从一次次的危难之中吸取教训\n"
                          "放下心中过多的执念与恶念......";
        }
        break;

    case Text4_Show:
        textAlpha = qMin(255, textAlpha + 15);
        if (textAlpha >= 255 && phaseTimer >= 8000) {
            endingState = Text4_FadeOut;
        }
        break;

    case Text4_FadeOut:
        textAlpha = qMax(0, textAlpha - 15);
        if (textAlpha <= 0) {
            endingTimer->stop();
            QTimer::singleShot(500, [=]() {
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
                } else {
                    MazeTower *mazeTower = new MazeTower();
                    mazeTower->show();
                }
                this->close();
            });
        }
        break;

    default:
        break;
    }

    update();
}

void Level4Window::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(rect(), QColor(40, 40, 45));
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

    if (m_isExitReached && endingState == VictoryText) {
        p.fillRect(rect(), QColor(0, 0, 0, 180));
        p.setPen(QColor(255, 255, 255));
        p.setFont(QFont("宋体", 32, QFont::Bold));
        p.drawText(rect(), Qt::AlignCenter, "Congratulations！\nYou'd passed this level！");
    }
    else if (m_isFailed) {
        p.fillRect(rect(), QColor(0, 0, 0, 180));
        p.setPen(QColor(255, 255, 255));
        p.setFont(QFont("宋体", 32, QFont::Bold));
        p.drawText(rect(), Qt::AlignCenter, "Unfortunately！\nYou'd been devoured by the abyss...");
    }

    if (endingState != Normal && endingState != VictoryText) {
        drawEnding(p);
    }
}

void Level4Window::drawMaze(QPainter &p) {
    p.setBrush(QColor(180, 180, 190));
    p.drawRect(100, 100, 1400, 700);

    p.setBrush(QColor(60, 60, 70));
    for (const auto& w : walls) {
        p.drawRect(w);
    }
}

void Level4Window::drawHUD(QPainter &p) {
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

void Level4Window::drawEnding(QPainter &p) {
    p.fillRect(rect(), QColor(0, 0, 0, bgAlpha));

    if (!currentText.isEmpty() && textAlpha > 0) {
        p.setPen(QColor(255, 255, 255, textAlpha));
        p.setFont(QFont("宋体", 24));

        QRect textRect(100, 200, 1400, 500);
        p.drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, currentText);
    }
}

void Level4Window::keyPressEvent(QKeyEvent *e) {
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

void Level4Window::keyReleaseEvent(QKeyEvent *e) {
    switch (e->key()) {
    case Qt::Key_W: case Qt::Key_Up: keyW = false; break;
    case Qt::Key_S: case Qt::Key_Down: keyS = false; break;
    case Qt::Key_A: case Qt::Key_Left: keyA = false; break;
    case Qt::Key_D: case Qt::Key_Right: keyD = false; break;
    }
}

void Level4Window::mouseMoveEvent(QMouseEvent *e) {
    qreal cx = playerX + PLAYER_SIZE/2;
    qreal cy = playerY + PLAYER_SIZE/2;
    aimAngle = qAtan2(e->y() - cy, e->x() - cx);
}
void Level4Window::mousePressEvent(QMouseEvent *) {
    if (!gameRunning || attackCooldown > 0) return;


    qreal cx = playerX + PLAYER_SIZE/2 +30;
    qreal cy = playerY + PLAYER_SIZE/2-15;

    qreal dx = qCos(aimAngle) * Projectile::SPEED;
    qreal dy = qSin(aimAngle) * Projectile::SPEED;
    projectiles.append(new Projectile(cx, cy, dx, dy));
    attackCooldown = 15;
}