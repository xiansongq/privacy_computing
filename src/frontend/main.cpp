#include <QApplication>
#include <QPushButton>

#include "include/mainwindow.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    //QPushButton button("Hello world!", nullptr);
    //button.resize(200, 100);
    // button.show();
    MainWindow w;
    w.setWindowTitle("PSI and PIR");
    w.show();
    return QApplication::exec();
}
