#ifndef LEVEL4_H
#define LEVEL4_H

#include <QWidget>
#include <QVector>
#include <QRectF>
#include <QTimer>
#include <QKeyEvent>
#include <QMouseEvent>
#include "gameentities.h"

enum EndingState {
    Normal,
    VictoryText,
    FadeToBlack,
    Text1_Show,
    Text1_FadeOut,
    Text2_Show,
    Text2_FadeOut,
    Text3_Show,
    Text3_FadeOut,
    Text4_Show,
    Text4_FadeOut,
    ReturnToMazeTower
};

class Level4Window : public QWidget
{
    Q_OBJECT
public:
    explicit Level4Window(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *) override;
    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mousePressEvent(QMouseEvent *) override;

private slots:
    void gameLoop();
    void checkCollisions();
    void gameOver(bool win);
    void nextEndingPhase();

private:
    void initMaze();
    bool isWall(qreal x, qreal y, qreal w, qreal h) const;
    void clampPlayer();
    void trySpawnEnemy();
    void spawnCollapseZone();
    void drawMaze(QPainter &p);
    void drawHUD(QPainter &p);
    void drawEnding(QPainter &p);

    static constexpr qreal PLAYER_SIZE = 25;
    static constexpr qreal DISPLAY_SIZE = 80;
    static constexpr qreal PLAYER_SPEED = 2.4;

    qreal playerX, playerY;
    int playerHp;
    bool playerAlive;

    bool keyW, keyA, keyS, keyD;
    qreal aimAngle;

    QVector<QRectF> walls;
    QRectF exitRect;

    QVector<Enemy*> enemies;
    QVector<Projectile*> projectiles;
    QVector<CollapseZone*> collapseZones;

    int spawnCooldownTimer;
    int attackCooldown;
    int collapseSpawnTimer;
    bool gameRunning;
    int gameTime;

    QTimer *timer;

    bool m_isExitReached = false;
    bool m_isFailed = false;

    // 结局动画
    EndingState endingState;
    QTimer *endingTimer;
    QString currentText;
    int textAlpha;
    int bgAlpha;
    int phaseTimer;
};

#endif