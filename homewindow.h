#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QWidget>
#include <QPushButton>

#include<QPainter>


class HomeWindow : public QWidget
{
    Q_OBJECT
public:
    explicit HomeWindow(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent*event);

private slots:
    void goStory();
    void goMazeTower();
    void goTrain();
    void toggleBGM();



private:

    QPushButton *btn1;
    QPushButton *btn2;
    QPushButton *btn3;
    QPushButton *btnMusic;
    void initBackgroundMusic();
};

#endif