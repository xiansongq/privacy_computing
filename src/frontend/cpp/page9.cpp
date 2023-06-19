#include "page9.h"
#include "ui_page9.h"

page9::page9(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::page9)
{
    ui->setupUi(this);
}

page9::~page9()
{
    delete ui;
}
