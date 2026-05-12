#include "gameruleswindow.h"
#include "homewindow.h"

GameRulesWindow::GameRulesWindow(QWidget *parent)
    : QWidget{parent}
{    setFixedSize(1600, 900);

    // 返回按钮
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

    connect(returnBtn, &QPushButton::clicked, this, &GameRulesWindow::backToHome);}
void GameRulesWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    QPixmap bgPix("://rule.jpg");  // 背景图片

    QPixmap scalePix = bgPix.scaled(
        this->size(),
        Qt::IgnoreAspectRatio,
        Qt::SmoothTransformation
        );

    painter.drawPixmap(0, 0, scalePix);
}

void GameRulesWindow::backToHome()
{
    HomeWindow *home = new HomeWindow();
    home->show();
    this->close();
}
