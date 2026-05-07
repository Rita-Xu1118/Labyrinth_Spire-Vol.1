#include "videowindow.h"
#include "homewindow.h"
#include <QUrl>

VideoWindow::VideoWindow(QWidget *parent) : QWidget(parent)
{
    setFixedSize(1600, 900);
    setStyleSheet("background-color: black;");

    //跳过开场按钮
    btnSkip = new QPushButton("跳过开场>>", this);
    btnSkip->setGeometry(1380, 20, 160, 40);
    btnSkip->setStyleSheet(R"(
        QPushButton {
            color: white;
            font-size: 14px;
            border: 1px solid white;
            border-radius: 10px;
            background: transparent;
        }
        QPushButton:hover {
            background-color: rgba(255,255,255,30);
        }
    )");

    //  视频
    player = new QMediaPlayer(this);
    player->setSource(QUrl::fromLocalFile("start.mp4"));

    connect(btnSkip, &QPushButton::clicked, this, &VideoWindow::skipVideo);

    player->play();
}

void VideoWindow::skipVideo()
{
    player->stop();
    HomeWindow *w = new HomeWindow;
    w->show();
    this->close();
}