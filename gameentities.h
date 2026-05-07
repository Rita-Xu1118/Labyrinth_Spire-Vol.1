#ifndef GAMEENTITIES_H
#define GAMEENTITIES_H

#include <QPainter>
#include <QRectF>
#include <QVector>
#include <QRandomGenerator>
#include <QtMath>

class Enemy {
public:
    Enemy(qreal x, qreal y);
    void update(qreal playerX, qreal playerY);
    void draw(QPainter &painter);
    QRectF getRect() const;
    void takeDamage(int dmg);
    bool isDead() const;
    bool isFullyVisible() const;
    bool canAttack() const;
    void resetAttackCooldown();

    qreal x, y;
    int hp;
    int maxHp;
    bool visible;
    int spawnTimer;
    bool spawning;
    int attackCooldown;

    static constexpr qreal SIZE = 32;
    static constexpr qreal SPEED = 1.9;
    static constexpr int ATTACK_COOLDOWN = 60;
};

class Projectile {
public:
    Projectile(qreal x, qreal y, qreal dx, qreal dy);
    void update();
    void draw(QPainter &painter);
    QRectF getRect() const;
    bool isOutOfBounds() const;

    qreal x, y;
    qreal dx, dy;
    static constexpr qreal SIZE = 8;
    static constexpr qreal SPEED = 8;
    bool alive;
};

class CollapseZone {
public:
    CollapseZone(qreal x, qreal y);
    void update();
    void draw(QPainter &painter);
    QRectF getRect() const;
    bool isCollapsed() const;

    qreal x, y;
    int warningTimer;
    bool collapsed;
    bool done;

    static constexpr qreal SIZE = 48;
};

#endif // GAMEENTITIES_H
