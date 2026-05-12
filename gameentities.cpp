#include "gameentities.h"

Enemy::Enemy(qreal x, qreal y)
    : x(x), y(y), visible(false), spawnTimer(120), spawning(true), attackCooldown(0)
{
    maxHp = QRandomGenerator::global()->bounded(3) + 2;
    hp = maxHp;
}

void Enemy::update(qreal playerX, qreal playerY) {
    if (spawning) {
        spawnTimer--;
        if (spawnTimer <= 0) {
            spawning = false;
            visible = true;
        }
        return;
    }

    if (!visible || hp <= 0) return;
    if (attackCooldown > 0) attackCooldown--;

    qreal dx = playerX - x;
    qreal dy = playerY - y;
    qreal len = qSqrt(dx*dx + dy*dy);

    if (len < 5) return;

    dx /= len;
    dy /= len;

    x += dx * SPEED;
    y += dy * SPEED;
}

bool Enemy::canAttack() const {
    return attackCooldown <= 0;
}

void Enemy::resetAttackCooldown() {
    attackCooldown = ATTACK_COOLDOWN;
}

void Enemy::draw(QPainter &painter) {
    if (hp <= 0) return;

    if (spawning) {
        int alpha = (spawnTimer % 20 < 10) ? 180 : 80;
        painter.setBrush(QColor(255, 0, 0, alpha));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(x - SIZE/2, y - SIZE/2, SIZE, SIZE);
        painter.setPen(QPen(QColor(255,0,0,200),2));
        painter.drawEllipse(x - SIZE/2 -5, y - SIZE/2 -5, SIZE+10, SIZE+10);
        return;
    }

    painter.setBrush(QColor(180,30,30));
    painter.setPen(QPen(QColor(120,20,20),2));
    painter.drawEllipse(x - SIZE/2, y - SIZE/2, SIZE, SIZE);

    painter.setBrush(Qt::yellow);
    painter.drawEllipse(QPointF(x-6,y-4),4,4);
    painter.drawEllipse(QPointF(x+6,y-4),4,4);
    painter.setBrush(Qt::black);
    painter.drawEllipse(QPointF(x-6,y-4),2,2);
    painter.drawEllipse(QPointF(x+6,y-4),2,2);
}

QRectF Enemy::getRect() const {
    return QRectF(x - SIZE/2, y - SIZE/2, SIZE, SIZE);
}

void Enemy::takeDamage(int dmg) { hp -= dmg; }
bool Enemy::isDead() const { return hp <=0; }
bool Enemy::isFullyVisible() const { return visible && !spawning; }

Projectile::Projectile(qreal x, qreal y, qreal dx, qreal dy)
    : x(x), y(y), dx(dx), dy(dy), alive(true) {}

void Projectile::update() {
    x += dx; y += dy;
    if (x<0||x>1600||y<0||y>900) alive = false;
}

void Projectile::draw(QPainter &painter) {
    painter.setBrush(QColor(173, 216, 230));  // 浅蓝色子弹
    painter.setPen(QPen(QColor(100, 150, 200), 1));  // 浅蓝边框
    painter.drawEllipse(x-SIZE/2, y-SIZE/2, SIZE, SIZE);
}

QRectF Projectile::getRect() const {
    return QRectF(x-SIZE/2,y-SIZE/2,SIZE,SIZE);
}

bool Projectile::isOutOfBounds() const {
    return !alive || x<0||x>1600||y<0||y>900;
}

CollapseZone::CollapseZone(qreal x, qreal y)
    : x(x), y(y), warningTimer(300), collapsed(false), done(false) {}

void CollapseZone::update() {
    if (done) return;
    warningTimer--;
    if (warningTimer <=0) {
        collapsed = true;
        done = true;
    }
}

void CollapseZone::draw(QPainter &painter) {
    if (done && !collapsed) return;
    if (!collapsed) {
        int a = warningTimer%20<10 ? 200 : 100;
        painter.setBrush(QColor(150,0,200,a));
        painter.setPen(QPen(QColor(200,50,255),3));
        painter.drawRect(x,y,SIZE,SIZE);
    } else {
        painter.setBrush(QColor(20,0,30));
        painter.setPen(QPen(QColor(80,0,100),2));
        painter.drawRect(x,y,SIZE,SIZE);
    }
}

QRectF CollapseZone::getRect() const { return QRectF(x,y,SIZE,SIZE); }
bool CollapseZone::isCollapsed() const { return collapsed; }