#ifndef LEVEL2_H
#define LEVEL2_H

#include <QWidget>

#include <QKeyEvent>
#include <QMouseEvent>
#include <QTimer>
#include <QVector>
#include <QRectF>
#include <QPainter>
#include<QSoundEffect>
#include "gameentities.h"

class Level2Window : public QWidget {
    Q_OBJECT
public:
    explicit Level2Window(QWidget *parent = nullptr);
    ~Level2Window(){
        // 清理敌人
        for (Enemy *e : enemies) {
            delete e;
        }
        enemies.clear();

        // 清理子弹
        for (Projectile *p : projectiles) {
            delete p;
        }
        projectiles.clear();

        // 清理坍塌区
        for (CollapseZone *z : collapseZones) {
            delete z;
        }
        collapseZones.clear();

        delete timer; // 清理定时器
    };

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void gameLoop();

private:
    void initMaze();
    void trySpawnEnemy();
    void spawnCollapseZone();
    void checkCollisions();
    void drawHUD(QPainter &p);
    void drawMaze(QPainter &p);
    void gameOver(bool win);
    void clampPlayer();
    bool isWall(qreal x, qreal y, qreal w, qreal h) const;
    bool m_isExitReached = false;
    bool m_isFailed = false;
    QSoundEffect *magicAttackSound = nullptr;

    QTimer *timer;

    qreal playerX, playerY;
    static constexpr qreal PLAYER_SIZE = 25;
    static constexpr qreal DISPLAY_SIZE = 80;
    static constexpr qreal PLAYER_SPEED = 2.4;
    int playerHp;
    bool playerAlive;

    bool keyW, keyA, keyS, keyD;
    qreal aimAngle;

    QVector<QRectF> walls;
    QRectF exitRect;

    QVector<Enemy*> enemies;
    int spawnCooldownTimer;

    QVector<Projectile*> projectiles;
    int attackCooldown;

    QVector<CollapseZone*> collapseZones;
    int collapseSpawnTimer;

    bool gameRunning;
    int gameTime;
    static constexpr int SPAWN_MIN = 120;
    static constexpr int SPAWN_MAX = 240;
};

#endif // LEVEL2_H
