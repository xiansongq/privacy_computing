/********************************************************************************
** Form generated from reading UI file 'page4.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PAGE4_H
#define UI_PAGE4_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
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

class Ui_page4
{
public:
    QWidget *centralwidget;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QWidget *widget;
    QLineEdit *file2input;
    QLabel *label_2;
    QLabel *label;
    QLineEdit *file1input;
    QPushButton *file1but;
    QPushButton *file2but;
    QPushButton *startcup;
    QWidget *page_2;
    QMenuBar *menubar;
    QMenu *menuPIRbe_c;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *page4)
    {
        if (page4->objectName().isEmpty())
            page4->setObjectName(QString::fromUtf8("page4"));
        page4->resize(800, 600);
        centralwidget = new QWidget(page4);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setGeometry(QRect(0, 0, 800, 600));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        widget = new QWidget(page);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(0, 0, 620, 221));
        file2input = new QLineEdit(widget);
        file2input->setObjectName(QString::fromUtf8("file2input"));
        file2input->setGeometry(QRect(90, 120, 370, 40));
        label_2 = new QLabel(widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 120, 81, 41));
        label_2->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\256\213\344\275\223\";"));
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 40, 81, 41));
        label->setToolTipDuration(-3);
        label->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\256\213\344\275\223\";\n"
"\n"
""));
        file1input = new QLineEdit(widget);
        file1input->setObjectName(QString::fromUtf8("file1input"));
        file1input->setGeometry(QRect(90, 40, 370, 40));
        file1but = new QPushButton(widget);
        file1but->setObjectName(QString::fromUtf8("file1but"));
        file1but->setGeometry(QRect(480, 40, 101, 31));
        file1but->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/png/img/choisefile.png"), QSize(), QIcon::Normal, QIcon::Off);
        file1but->setIcon(icon);
        file2but = new QPushButton(widget);
        file2but->setObjectName(QString::fromUtf8("file2but"));
        file2but->setGeometry(QRect(480, 120, 101, 31));
        file2but->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        file2but->setIcon(icon);
        startcup = new QPushButton(widget);
        startcup->setObjectName(QString::fromUtf8("startcup"));
        startcup->setGeometry(QRect(260, 180, 151, 31));
        startcup->setStyleSheet(QString::fromUtf8("font: 11pt \"\346\245\267\344\275\223\";"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/png/img/1.png"), QSize(), QIcon::Normal, QIcon::Off);
        startcup->setIcon(icon1);
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        stackedWidget->addWidget(page_2);
        page4->setCentralWidget(centralwidget);
        menubar = new QMenuBar(page4);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        menuPIRbe_c = new QMenu(menubar);
        menuPIRbe_c->setObjectName(QString::fromUtf8("menuPIRbe_c"));
        page4->setMenuBar(menubar);
        statusbar = new QStatusBar(page4);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        page4->setStatusBar(statusbar);

        menubar->addAction(menuPIRbe_c->menuAction());

        retranslateUi(page4);

        QMetaObject::connectSlotsByName(page4);
    } // setupUi

    void retranslateUi(QMainWindow *page4)
    {
        page4->setWindowTitle(QApplication::translate("page4", "MainWindow", nullptr));
        label_2->setText(QApplication::translate("page4", "\346\225\260\346\215\256\345\272\223\346\226\207\344\273\266\357\274\232", nullptr));
        label->setText(QApplication::translate("page4", "\345\276\205\346\237\245\350\257\242\345\210\227\350\241\250\357\274\232", nullptr));
        file1but->setText(QApplication::translate("page4", "\351\200\211\346\213\251\346\226\207\344\273\2661", nullptr));
        file2but->setText(QApplication::translate("page4", "\351\200\211\346\213\251\346\226\207\344\273\2662", nullptr));
        startcup->setText(QApplication::translate("page4", "\345\274\200\345\247\213\350\256\241\347\256\227", nullptr));
        menuPIRbe_c->setTitle(QApplication::translate("page4", "PIR\346\234\254\345\234\260\346\237\245\350\257\242", nullptr));
    } // retranslateUi

};

namespace Ui {
    class page4: public Ui_page4 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PAGE4_H
