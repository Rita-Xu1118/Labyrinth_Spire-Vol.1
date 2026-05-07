#ifndef TRAININGWINDOW_H
#define TRAININGWINDOW_H

#include <QWidget>
#include <QTimer>
#include <QVector>
#include <QRectF>
#include <QPainter>
#include <QPushButton>
#include <QLabel>
#include "gameentities.h"

class TrainingWindow : public QWidget
{
    Q_OBJECT
public:
    explicit TrainingWindow(QWidget *parent = nullptr);
    ~TrainingWindow();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void gameLoop();
    void startAnimation();  // 开场动画
    void spawnEnemy();      // 刷怪
    void checkCollisions();
    void backToHome();      // 返回主页

private:
    void drawUI(QPainter &p);
    void tryConvertToKey(); // 10勇气+10悲悯 = 1心钥

    // 开场动画
    int animationStep;
    QTimer *animTimer;

    // 游戏
    QTimer *timer;
    QTimer *spawnTimer;

    // 玩家
    qreal px, py;
    qreal aimAngle;
    bool w,a,s,d;
    static constexpr qreal SPEED = 2.5;
    static constexpr qreal SIZE = 28;

    // 怪物 & 子弹
    QVector<Enemy*> enemies;
    QVector<Projectile*> projectiles;

    // 掉落物
    struct Drop {
        enum Type { Courage, Mercy } type;
        qreal x, y;
        static constexpr qreal SIZE = 16;
    };
    QVector<Drop> drops;

    // 数值
    int courage;   // 勇气
    int mercy;     // 悲悯
    int keyCount;   // 心钥（同步给MazeTower）

    // UI
    QPushButton *backBtn;
    QPixmap bgImage;

    // 冷却
    int attackCd;
};

#endif // TRAININGWINDOW_H
