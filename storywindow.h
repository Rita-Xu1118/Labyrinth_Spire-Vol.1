#ifndef STORYWINDOW_H
#define STORYWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QPainter>

class StoryWindow : public QWidget
{
    Q_OBJECT
public:
    explicit StoryWindow(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);

private slots:
    void backToHome();

private:
    QPushButton *returnBtn;
};

#endif