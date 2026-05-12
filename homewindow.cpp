#include "homewindow.h"
#include "mazetower.h"
#include <QMessageBox>
#include <QFont>
#include <QUrl>
#include "trainingwindow.h"
#include "audiomanager.h"
#include "storywindow.h"
#include "gameruleswindow.h"


//bgm
void HomeWindow::initBackgroundMusic()
{
    AudioManager::instance()->play("qrc:/bgm.mp3");
}


//添加背景图片
void HomeWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QWidget::paintEvent(event);

    QPainter painter(this);
    QPixmap bgPix("://1.bmp");


    // 自适应窗口大小
    QPixmap scalePix = bgPix.scaled(
        this->size(),
        Qt::IgnoreAspectRatio,
        Qt::SmoothTransformation
        );

    painter.drawPixmap(0, 0, scalePix);
}



HomeWindow::HomeWindow(QWidget *parent) : QWidget(parent)
{
    setFixedSize(1600, 900);
    setStyleSheet("background-color: #111111;");



    //准备添加背景音乐
    initBackgroundMusic();


    // 按钮位置和字体
    QFont f;
    f.setPointSize(28);
    f.setFamily("宋体");

    btn1 = new QPushButton("背景故事", this);
    btn1->setGeometry(600, 500, 400, 100);
    btn1->setFont(f);

    btn2 = new QPushButton("前往迷宫塔", this);
    btn2->setGeometry(600, 620, 400, 100);
    btn2->setFont(f);

    btn3 = new QPushButton("训练场", this);
    btn3->setGeometry(600, 740, 400, 100);
    btn3->setFont(f);


    btnMusic = new QPushButton("🔊 音乐", this);
    btnMusic->setGeometry(1350, 800, 180, 60);

    btnRules = new QPushButton("📖 玩法", this);
    btnRules->setGeometry(1380, 20, 150, 50);

    // 按钮样式
    QString btnStyle = R"(
        QPushButton {
            background-color: rgba(255,255,255,0);
            color: #DCDCDC;
            border-radius: 18px;
            border: 2px solid rgba(220,220,220,160);
        }
        QPushButton:hover {
            background-color: rgba(255,255,255,30);
            color: white;
        }
    )";

    btn1->setStyleSheet(btnStyle);
    btn2->setStyleSheet(btnStyle);
    btn3->setStyleSheet(btnStyle);
    btnMusic->setStyleSheet(btnStyle);
    btnRules->setStyleSheet(btnStyle);

    // 连接按钮和相应页面（函数声明）
    connect(btn1, &QPushButton::clicked, this, &HomeWindow::goStory);
    connect(btn2, &QPushButton::clicked, this, &HomeWindow::goMazeTower);
    connect(btn3, &QPushButton::clicked, this, &HomeWindow::goTrain);
    connect(btnMusic, &QPushButton::clicked, this, &HomeWindow::toggleBGM);
    connect(btnRules, &QPushButton::clicked, this, &HomeWindow::goGameRules);

}



void HomeWindow::toggleBGM()
{
    if (AudioManager::instance()->isPlaying()) {
        AudioManager::instance()->pause();
        btnMusic->setText("🔇 音乐");
    } else {
        AudioManager::instance()->resume();
        btnMusic->setText("🔊 音乐");
    }
}
//连接按钮和相应页面（函数定义）

void HomeWindow::goStory()
{
    StoryWindow *sw = new StoryWindow();
    sw->show();
    close();
}

void HomeWindow::goMazeTower(){//前往选关页面


    MazeTower *w=new MazeTower;
    w->show();
    close();
}

void HomeWindow::goTrain(){//前往训练场


    TrainingWindow *tw = new TrainingWindow();
    tw->show();
    close();
}

void HomeWindow::goGameRules()
{
    GameRulesWindow *rw = new GameRulesWindow();
    rw->show();
    close();
}
