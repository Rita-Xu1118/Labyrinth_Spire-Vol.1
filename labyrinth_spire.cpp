#include "labyrinth_spire.h"
#include "./ui_labyrinth_spire.h"

Labyrinth_Spire::Labyrinth_Spire(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Labyrinth_Spire)
{
    ui->setupUi(this);
}

Labyrinth_Spire::~Labyrinth_Spire()
{
    delete ui;
}
