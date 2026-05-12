#ifndef TRAININGWINDOW_H
#define TRAININGWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QTimer>
#include <QPainter>
#include <QKeyEvent>
#include <QMouseEvent>
#include "gameentities.h"

class TrainingWindow : public QWidget
{
    Q_OBJECT
public:
    explicit TrainingWindow(QWidget *parent = nullptr);
    ~TrainingWindow();

protected:
    void paintEvent(QPaintEvent *) override;
    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mousePressEvent(QMouseEvent *) override;

private slots:
    void startAnimation();
    void gameLoop();
    void spawnEnemy();
    void checkCollisions();
    void tryConvertToKey();
    void backToHome();

private:
    void drawUI(QPainter &p);

    QPixmap bgImage;
    QPushButton *backBtn;
    QTimer *animTimer;
    QTimer *timer;
    QTimer *spawnTimer;

    int animationStep;
    qreal px, py;
    qreal aimAngle;
    bool w, a, s, d;
    int attackCd;
    int courage, mercy;

    QVector<Enemy*> enemies;
    QVector<Projectile*> projectiles;

    struct Drop {
        qreal x, y;
        enum Type { Courage, Mercy } type;
        static constexpr qreal SIZE = 16;
    };
    QVector<Drop> drops;

    static constexpr qreal SIZE = 140;           // 碰撞框
    static constexpr qreal DISPLAY_SIZE = 160;  // 显示大小
    static constexpr qreal SPEED = 4;
};

#endif
