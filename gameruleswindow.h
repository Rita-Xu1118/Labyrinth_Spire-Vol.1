#ifndef GAMERULESWINDOW_H
#define GAMERULESWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QPainter>

class GameRulesWindow : public QWidget
{
    Q_OBJECT
public:
    explicit GameRulesWindow(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);

private slots:
    void backToHome();

private:
    QPushButton *returnBtn;
};

#endif

