#ifndef VIDEOWINDOW_H
#define VIDEOWINDOW_H

#include <QWidget>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QPushButton>

class VideoWindow : public QWidget
{
    Q_OBJECT
public:
    explicit VideoWindow(QWidget *parent = nullptr);

private slots:
    void skipVideo(); // 跳过视频的函数

private:
    QMediaPlayer *player;     // 视频播放器
    QVideoWidget *videoWidget;// 视频显示窗口
    QPushButton *btnSkip;     // 跳过按钮
};

#endif // VIDEOWINDOW_H
