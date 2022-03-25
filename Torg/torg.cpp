#include "torg.h"
#include "ui_torg.h"

Torg::Torg(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Torg)
{
    ui->setupUi(this);
}

Torg::~Torg()
{
    delete ui;
}

