#include "mazetower.h"
#include "level1.h"
#include "level2.h"
#include "level3.h"
#include "level4.h"
#include "homewindow.h"
#include <QFont>
#include <QTimer>
#include"globaldata.h"

MazeTower::MazeTower(QWidget *parent) : QWidget(parent)
{
    setFixedSize(1600, 900);

    keyCount = GlobalData::keyCount;

    setStyleSheet(R"(
        QWidget {
            background-color: #2B2B2B;
            background-repeat: repeat;
        }
    )");

    // 左上角心钥 灰白色框
    keyLabel = new QLabel(this);
    keyLabel->setGeometry(30, 30, 220, 50);
    keyLabel->setStyleSheet(R"(
        QLabel {
            color: #EAEAEA;
            font-size: 24px;
            font-weight: bold;
            background-color: rgba(80,80,80,120);
            border-radius: 10px;
            padding: 8px 15px;
            border: 2px solid #B0B0B0;
        }
    )");
    keyLabel->setText("🔑 心钥：" + QString::number(keyCount));

    // 提示文字
    hintLabel = new QLabel(this);
    hintLabel->setAlignment(Qt::AlignCenter);
    hintLabel->setGeometry(400, 680, 800, 60);
    hintLabel->setStyleSheet(R"(
        QLabel {
            color: #FF8888;
            font-size: 30px;
            font-weight: bold;
        }
    )");
    hintLabel->clear();

    hintTimer = new QTimer(this);
    hintTimer->setSingleShot(true);
    connect(hintTimer, &QTimer::timeout, this, &MazeTower::hideHint);

    //右上角返回按钮
    returnBtn = new QPushButton("返回", this);
    returnBtn->setGeometry(1420, 30, 130, 50);
    QString returnStyle = R"(
        QPushButton {
            background-color: transparent;
            color: white;
            border: 2px solid white;
            border-radius: 10px;
            font-size: 22px;
        }
        QPushButton:hover {
            background-color: rgba(255,255,255,20);
        }
        QPushButton:pressed {
            background-color: rgba(255,255,255,40);
        }
    )";
    returnBtn->setStyleSheet(returnStyle);
    // 返回主页面
    connect(returnBtn, &QPushButton::clicked, this, &MazeTower::backToHome);

    // 关卡按钮
    level1 = new QPushButton("1", this);
    level2 = new QPushButton("2", this);
    level3 = new QPushButton("3", this);
    level4 = new QPushButton("4", this);

    QString btnStyle = R"(
        QPushButton {
            background-color: rgba(255,255,255,0);
            color: white;
            border-radius: 60px;
            border: 3px solid white;
            font-size: 32px;
            min-width: 120px;
            min-height: 120px;
        }
        QPushButton:hover {
            background-color: rgba(255,255,255,20);
        }
        QPushButton:pressed {
            background-color: rgba(255,255,255,40);
        }
    )";

    level1->setStyleSheet(btnStyle);
    level2->setStyleSheet(btnStyle);
    level3->setStyleSheet(btnStyle);
    level4->setStyleSheet(btnStyle);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addStretch();
    layout->addWidget(level1);
    layout->addSpacing(53);
    layout->addWidget(level2);
    layout->addSpacing(53);
    layout->addWidget(level3);
    layout->addSpacing(53);
    layout->addWidget(level4);
    layout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch();
    mainLayout->addLayout(layout);
    mainLayout->addStretch();
    mainLayout->setContentsMargins(0,0,0,120);

    connect(level1, &QPushButton::clicked, this, &MazeTower::openLevel1);
    connect(level2, &QPushButton::clicked, this, &MazeTower::openLevel2);
    connect(level3, &QPushButton::clicked, this, &MazeTower::openLevel3);
    connect(level4, &QPushButton::clicked, this, &MazeTower::openLevel4);
}
//根据心钥数量判断是否可以游戏
bool MazeTower::canPlay() {
    return keyCount > 0;
}
//若开始游戏，减少一个心钥，并同步全局
void MazeTower::consumeKey() {
    keyCount--;
    GlobalData::keyCount = keyCount; // 同步全局
    keyLabel->setText("🔑 心钥：" + QString::number(keyCount));
}

void MazeTower::showHint() {
    hintLabel->setText("心钥不足，请移步训练场，静心历练");
    hintTimer->start(2000);
}

void MazeTower::hideHint() {
    hintLabel->clear();
}

// 按钮分别前往各自页面
void MazeTower::backToHome()
{
    HomeWindow *home = new HomeWindow();
    home->show();
    this->close();
}

void MazeTower::openLevel1() {
    if (!canPlay()) { showHint(); return; }
    consumeKey();
    Level1Window *w = new Level1Window();
    w->show();
    this->close();
}

void MazeTower::openLevel2() {
    if (!canPlay()) { showHint(); return; }
    consumeKey();
    Level2Window *w = new Level2Window();
    w->show();
    this->close();
}

void MazeTower::openLevel3() {
    if (!canPlay()) { showHint(); return; }
    consumeKey();
    Level3Window *w = new Level3Window();
    w->show();
    this->close();
}

void MazeTower::openLevel4() {
    if (!canPlay()) { showHint(); return; }
    consumeKey();
    Level4Window *w = new Level4Window();
    w->show();
    this->close();
}