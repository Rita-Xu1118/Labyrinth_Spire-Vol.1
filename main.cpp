#include "homewindow.h"
#include <QApplication>
#include"globaldata.h"
int GlobalData::keyCount = 0;

// 整个游戏的入口：打开主界面
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HomeWindow w;
    w.show();
    return a.exec();
}