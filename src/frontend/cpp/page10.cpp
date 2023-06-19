#include "page10.h"
#include "ui_page10.h"

page10::page10(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::page10)
{
    ui->setupUi(this);
}

page10::~page10()
{
    delete ui;
}
