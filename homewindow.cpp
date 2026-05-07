#include "homewindow.h"
#include "mazetower.h"
#include <QMessageBox>
#include <QFont>
#include <QUrl>
#include "trainingwindow.h"

HomeWindow::HomeWindow(QWidget *parent) : QWidget(parent)
{
    setFixedSize(1600, 900);
    setStyleSheet("background-color: #111111;");

    //准备添加背景图片

    //准备添加背景音乐

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

    // 音乐开关按钮
    btnMusic = new QPushButton("🔊 音乐", this);
    btnMusic->setGeometry(1350, 800, 180, 60);

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
    btnMusic->setStyleSheet(btnStyle);//想实现音乐开关按钮但是音乐未实现

    // 连接按钮和相应页面（函数声明）
    connect(btn1, &QPushButton::clicked, this, &HomeWindow::goStory);
    connect(btn2, &QPushButton::clicked, this, &HomeWindow::goMazeTower);
    connect(btn3, &QPushButton::clicked, this, &HomeWindow::goTrain);

}


// 没有实现，bgm

//连接按钮和相应页面（函数定义）
void HomeWindow::goStory(){//未编译，后期会新建文件导入图片
    QMessageBox::information(this,"","背景故事");
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