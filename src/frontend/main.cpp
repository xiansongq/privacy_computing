#include <QApplication>
#include <QPushButton>
#include<QSize>
#include<QStyle>
#include "include/mainwindow.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MainWindow w;
    w.setWindowTitle("PSI and PIR");
    w.setWindowIcon(QIcon("://img/index.png"));
    w.resize(QSize(820,600));
    w.show();
    return QApplication::exec();
}
