#include "trainingwindow.h"
#include "homewindow.h"
#include "mazetower.h"
#include <QFont>
#include <QRandomGenerator>
#include <QTimer>
#include <QtMath>
#include <QKeyEvent>
#include <QMouseEvent>
#include"globaldata.h"

TrainingWindow::TrainingWindow(QWidget *parent)
    : QWidget(parent),
    animationStep(0),
    courage(0),
    mercy(0),
    keyCount(GlobalData::keyCount),
    attackCd(0)
{
    setFixedSize(1600, 900);
    setMouseTracking(true);
    setStyleSheet("background:black;");
    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    bgImage.load("bg_train.png");

    px = width()/2 - SIZE/2;
    py = height()/2 - SIZE/2;

    w=a=s=d=false;
    aimAngle = 0;

    backBtn = new QPushButton("返回", this);
    backBtn->setGeometry(1420, 30, 130, 50);
    backBtn->setStyleSheet(R"(
        QPushButton {
            background:transparent; color:white; border:2px solid white;
            border-radius:10px; font-size:22px;
        }
        QPushButton:hover { background:rgba(255,255,255,20); }
    )");
    connect(backBtn, &QPushButton::clicked, this, &TrainingWindow::backToHome);

    animTimer = new QTimer(this);
    connect(animTimer, &QTimer::timeout, this, &TrainingWindow::startAnimation);
    animTimer->start(800);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TrainingWindow::gameLoop);

    spawnTimer = new QTimer(this);
    connect(spawnTimer, &QTimer::timeout, this, &TrainingWindow::spawnEnemy);
    spawnTimer->start(7000);
}

TrainingWindow::~TrainingWindow()
{
    qDeleteAll(enemies); enemies.clear();
    qDeleteAll(projectiles); projectiles.clear();
}
//开场动画
void TrainingWindow::startAnimation()
{
    animationStep++;
    if (animationStep == 1) {
    } else if (animationStep == 2) {
    } else if (animationStep == 3) {
    } else if (animationStep == 4) {
        setStyleSheet("");
        timer->start(16);
        animTimer->stop();
    }
    update();
}
//敌人刷新
void TrainingWindow::spawnEnemy()
{
    int count = QRandomGenerator::global()->bounded(2,4);
    for (int i=0; i<count; i++) {
        qreal x, y;
        int side = QRandomGenerator::global()->bounded(4);

        if (side == 0) { x = QRandomGenerator::global()->bounded(width()); y = -50; }
        else if (side ==1) {x=width()+50; y=QRandomGenerator::global()->bounded(height());}
        else if (side ==2) {x=QRandomGenerator::global()->bounded(width()); y=height()+50;}
        else {x=-50; y=QRandomGenerator::global()->bounded(height());}

        auto e = new Enemy(x,y);
        e->hp = QRandomGenerator::global()->bounded(2,5);
        enemies.append(e);
    }
}
//战斗程序
void TrainingWindow::gameLoop()
{
    qreal nx = px, ny = py;
    if(w) ny-=SPEED; if(s) ny+=SPEED;
    if(a) nx-=SPEED; if(d) nx+=SPEED;

    if(nx>0 && nx+SIZE<width()) px=nx;
    if(ny>0 && ny+SIZE<height()) py=ny;

    for (auto e : enemies) e->update(px+SIZE/2, py+SIZE/2);
    for (auto p : projectiles) p->update();

    for (int i=projectiles.size()-1; i>=0; i--) {
        if (projectiles[i]->isOutOfBounds()) {
            delete projectiles[i]; projectiles.removeAt(i);
        }
    }

    if (attackCd>0) attackCd--;
    checkCollisions();
    tryConvertToKey();
    update();
}

void TrainingWindow::checkCollisions()
{
    QRectF player(px,py,SIZE,SIZE);

    for (int i=projectiles.size()-1; i>=0; i--) {
        auto p = projectiles[i];
        for (int j=enemies.size()-1; j>=0; j--) {
            auto e = enemies[j];
            if (p->getRect().intersects(e->getRect())) {
                e->takeDamage(1);
                delete projectiles[i]; projectiles.removeAt(i);

                if (e->isDead()) {
                    Drop d;
                    d.x = e->x + Enemy::SIZE/2 - Drop::SIZE/2;
                    d.y = e->y + Enemy::SIZE/2 - Drop::SIZE/2;
                    d.type = QRandomGenerator::global()->bounded(2) == 0 ? Drop::Courage : Drop::Mercy;
                    drops.append(d);

                    delete enemies[j]; enemies.removeAt(j);
                }
                break;
            }
        }
    }

    for (int i=drops.size()-1; i>=0; i--) {
        auto &d = drops[i];
        QRectF r(d.x,d.y,Drop::SIZE,Drop::SIZE);
        if (player.intersects(r)) {
            if (d.type == Drop::Courage) courage++;
            else mercy++;
            drops.removeAt(i);
        }
    }
}
//掉落物转化为心钥程序
void TrainingWindow::tryConvertToKey()
{
    if (courage >= 10 && mercy >= 10) {
        courage -= 10;
        mercy -= 10;
        keyCount += 1;
    }
     GlobalData::keyCount = keyCount;
}
//设计物品栏
void TrainingWindow::drawUI(QPainter &p)
{
    p.setPen(Qt::white);
    p.setFont(QFont("Arial",24));
    p.drawText(30, 40, QString("勇气: %1").arg(courage));
    p.drawText(30, 80, QString("悲悯: %1").arg(mercy));
    p.drawText(30, 120, QString("🔑心钥: %1").arg(keyCount));
}
//ok呀把所有东西画出来
void TrainingWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    if (animationStep < 4) {
        p.fillRect(rect(), Qt::black);
        p.setPen(Qt::white);
        p.setFont(QFont("Arial", 60, QFont::Bold));
        if (animationStep == 1)
            p.drawText(rect(), Qt::AlignCenter, "Are you ready?");
        else if (animationStep == 3)
            p.drawText(rect(), Qt::AlignCenter, "Go!");
        return;
    }

    if (!bgImage.isNull())
        p.drawPixmap(rect(), bgImage);
    else
        p.fillRect(rect(), QColor(30,30,40));

    for (auto e : enemies) e->draw(p);

    for (auto &d : drops) {
        p.setBrush(d.type == Drop::Courage ? Qt::yellow : Qt::cyan);
        p.drawEllipse(d.x, d.y, Drop::SIZE, Drop::SIZE);
    }

    p.setBrush(QColor(139,90,43));
    p.drawRoundedRect(px,py,SIZE,SIZE,4,4);

    qreal cx = px+SIZE/2, cy=py+SIZE/2;
    qreal dx = qCos(aimAngle);
    qreal dy = qSin(aimAngle);
    p.setPen(QPen(Qt::yellow, 4));
    p.drawLine(cx,cy,cx+dx*30,cy+dy*30);

    for (auto pr : projectiles) pr->draw(p);
    drawUI(p);
}
//键盘操纵
void TrainingWindow::keyPressEvent(QKeyEvent *e)
{
    switch(e->key()) {
    case Qt::Key_W: case Qt::Key_Up: w=true; break;
    case Qt::Key_S: case Qt::Key_Down: s=true; break;
    case Qt::Key_A: case Qt::Key_Left: a=true; break;
    case Qt::Key_D: case Qt::Key_Right: d=true; break;
    case Qt::Key_Escape: backToHome(); break;
    }
}

void TrainingWindow::keyReleaseEvent(QKeyEvent *e)
{
    switch(e->key()) {
    case Qt::Key_W:case Qt::Key_Up: w=false; break;
    case Qt::Key_S:case Qt::Key_Down: s=false; break;
    case Qt::Key_A:case Qt::Key_Left: a=false; break;
    case Qt::Key_D:case Qt::Key_Right: d=false; break;
    }
}
//鼠标操纵
void TrainingWindow::mouseMoveEvent(QMouseEvent *e)
{
    qreal cx=px+SIZE/2, cy=py+SIZE/2;
    aimAngle = qAtan2(e->y()-cy, e->x()-cx);
}

void TrainingWindow::mousePressEvent(QMouseEvent *)
{
    if (attackCd>0) return;
    attackCd=15;
    qreal cx=px+SIZE/2, cy=py+SIZE/2;
    qreal dx=qCos(aimAngle)*Projectile::SPEED;
    qreal dy=qSin(aimAngle)*Projectile::SPEED;
    projectiles.append(new Projectile(cx,cy,dx,dy));
}
//返回
void TrainingWindow::backToHome()
{
    // 把心钥同步到全局
    GlobalData::keyCount = keyCount;

    HomeWindow *h = new HomeWindow();
    h->show();
    this->close();
}