#ifndef LABYRINTH_SPIRE_H
#define LABYRINTH_SPIRE_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui {
class Labyrinth_Spire;
}
QT_END_NAMESPACE

class Labyrinth_Spire : public QWidget
{
    Q_OBJECT

public:
    explicit Labyrinth_Spire(QWidget *parent = nullptr);
    ~Labyrinth_Spire() override;


private:
    Ui::Labyrinth_Spire *ui;
};
#endif // LABYRINTH_SPIRE_H
