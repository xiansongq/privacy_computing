/********************************************************************************
** Form generated from reading UI file 'page5.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PAGE5_H
#define UI_PAGE5_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_page5
{
public:
    QWidget *centralwidget;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QWidget *page_2;
    QWidget *widget;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *ip;
    QLineEdit *port;
    QLabel *label_4;
    QLineEdit *filepath;
    QPushButton *choisefile;
    QCheckBox *isserver;
    QPushButton *startcpu;
    QLabel *label_3;
    QMenuBar *menubar;
    QMenu *menuPIR;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *page5)
    {
        if (page5->objectName().isEmpty())
            page5->setObjectName(QString::fromUtf8("page5"));
        page5->resize(800, 600);
        centralwidget = new QWidget(page5);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setGeometry(QRect(0, 0, 800, 600));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        widget = new QWidget(page_2);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(0, 49, 620, 151));
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(50, 10, 30, 30));
        label->setStyleSheet(QString::fromUtf8("font: 25 14pt \"Adobe \345\256\213\344\275\223 Std L\";\n"
"color:rgb(0, 85, 255);"));
        label_2 = new QLabel(widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(290, 10, 60, 30));
        label_2->setStyleSheet(QString::fromUtf8("font: 25 14pt \"Adobe \345\256\213\344\275\223 Std L\";\n"
"color:rgb(0, 85, 255)"));
        ip = new QLineEdit(widget);
        ip->setObjectName(QString::fromUtf8("ip"));
        ip->setGeometry(QRect(90, 10, 180, 30));
        ip->setText(QString::fromUtf8(""));
        port = new QLineEdit(widget);
        port->setObjectName(QString::fromUtf8("port"));
        port->setGeometry(QRect(350, 10, 74, 30));
        label_4 = new QLabel(widget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(30, 50, 60, 20));
        label_4->setStyleSheet(QString::fromUtf8("font: 25 14pt \"Adobe \345\256\213\344\275\223 Std L\";\n"
"color:rgb(0, 85, 255);"));
        filepath = new QLineEdit(widget);
        filepath->setObjectName(QString::fromUtf8("filepath"));
        filepath->setGeometry(QRect(90, 50, 265, 30));
        filepath->setText(QString::fromUtf8(""));
        choisefile = new QPushButton(widget);
        choisefile->setObjectName(QString::fromUtf8("choisefile"));
        choisefile->setGeometry(QRect(390, 50, 100, 30));
        choisefile->setStyleSheet(QString::fromUtf8("\n"
"font: 25 10pt \"Microsoft YaHei\";"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/png/img/choisefile.png"), QSize(), QIcon::Normal, QIcon::Off);
        choisefile->setIcon(icon);
        isserver = new QCheckBox(widget);
        isserver->setObjectName(QString::fromUtf8("isserver"));
        isserver->setGeometry(QRect(450, 10, 131, 31));
        isserver->setStyleSheet(QString::fromUtf8("font: 25 14pt \"Adobe \345\256\213\344\275\223 Std L\";\n"
"color:rgb(255, 0, 0)"));
        startcpu = new QPushButton(widget);
        startcpu->setObjectName(QString::fromUtf8("startcpu"));
        startcpu->setGeometry(QRect(260, 100, 100, 30));
        startcpu->setStyleSheet(QString::fromUtf8("font: 25 10pt \"Microsoft YaHei\";"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/png/img/start.png"), QSize(), QIcon::Normal, QIcon::Off);
        startcpu->setIcon(icon1);
        label_3 = new QLabel(page_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 10, 620, 30));
        label_3->setStyleSheet(QString::fromUtf8("font: 25 9pt \"Adobe \345\256\213\344\275\223 Std L\";\n"
"color:rgb(255, 0, 0)"));
        label_3->setTextFormat(Qt::RichText);
        stackedWidget->addWidget(page_2);
        page5->setCentralWidget(centralwidget);
        menubar = new QMenuBar(page5);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        menuPIR = new QMenu(menubar);
        menuPIR->setObjectName(QString::fromUtf8("menuPIR"));
        page5->setMenuBar(menubar);
        statusbar = new QStatusBar(page5);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        page5->setStatusBar(statusbar);

        menubar->addAction(menuPIR->menuAction());

        retranslateUi(page5);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(page5);
    } // setupUi

    void retranslateUi(QMainWindow *page5)
    {
        page5->setWindowTitle(QApplication::translate("page5", "MainWindow", nullptr));
        label->setText(QApplication::translate("page5", "IP\357\274\232", nullptr));
        label_2->setText(QApplication::translate("page5", "\347\253\257\345\217\243\357\274\232", nullptr));
        label_4->setText(QApplication::translate("page5", "\346\226\207\344\273\266\357\274\232", nullptr));
        choisefile->setText(QApplication::translate("page5", "\351\200\211\346\213\251\346\226\207\344\273\266", nullptr));
        isserver->setText(QApplication::translate("page5", "\346\237\245\350\257\242\346\226\271\357\274\237", nullptr));
        startcpu->setText(QApplication::translate("page5", "\345\274\200\345\247\213\346\237\245\350\257\242", nullptr));
        label_3->setText(QApplication::translate("page5", "\346\217\220\347\244\272\357\274\232\350\201\224\346\234\272\346\211\247\350\241\214\344\270\244\346\226\271PIR\357\274\214\351\246\226\345\205\210\350\246\201\350\276\223\345\205\245\345\257\271\346\226\271ip\345\234\260\345\235\200\345\222\214\347\253\257\345\217\243\345\217\267\357\274\214\351\234\200\350\246\201\346\214\207\346\230\216\350\260\201\346\230\257\346\237\245\350\257\242\346\226\271\343\200\202", nullptr));
        menuPIR->setTitle(QApplication::translate("page5", "PIR\347\264\242\345\274\225\350\201\224\346\234\272\346\237\245\350\257\242", nullptr));
    } // retranslateUi

};

namespace Ui {
    class page5: public Ui_page5 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PAGE5_H
