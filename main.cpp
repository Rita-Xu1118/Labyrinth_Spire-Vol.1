#include "videowindow.h"
#include <QApplication>
#include"globaldata.h"
int GlobalData::keyCount = 0;

// 整个游戏的入口：一运行就打开开场视频
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VideoWindow w;  // 打开开场视频页面
    w.show();
    return a.exec();
}