#ifndef MAZETOWER_H
#define MAZETOWER_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>

class MazeTower : public QWidget
{
    Q_OBJECT
public:
    explicit MazeTower(QWidget *parent = nullptr);

private slots:
    void openLevel1();
    void openLevel2();
    void openLevel3();
    void openLevel4();
    void showHint();       // 显示提示
    void hideHint();      // 隐藏提示
    void backToHome();
private:
    bool canPlay();       // 判断是否有心钥
    void consumeKey();    // 消耗心钥

    QPushButton *level1;
    QPushButton *level2;
    QPushButton *level3;
    QPushButton *level4;
    QPushButton *returnBtn;

    // 心钥系统
    QLabel *keyLabel;       // 左上角 钥匙: 数量
    QLabel *hintLabel;      // 提示文字
    int keyCount;           // 心钥数量
    QTimer *hintTimer;      // 提示计时
};

#endif // MAZETOWER_H
