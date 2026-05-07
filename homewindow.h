#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QMediaPlayer>//视频，还没实现

class HomeWindow : public QWidget
{
    Q_OBJECT
public:
    explicit HomeWindow(QWidget *parent = nullptr);

private slots:
    void goStory();
    void goMazeTower();
    void goTrain();
    void toggleBGM();

private:
    QMediaPlayer *player;  // 视频（无声）(目前还没实现）
    QMediaPlayer *bgm;     // 背景音乐（目前还没实现）

    QPushButton *btn1;
    QPushButton *btn2;
    QPushButton *btn3;
    QPushButton *btnMusic;
};

#endif