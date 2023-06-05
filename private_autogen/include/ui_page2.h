/********************************************************************************
** Form generated from reading UI file 'page2.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PAGE2_H
#define UI_PAGE2_H

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

class Ui_page2
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
    QMenu *menu_PSI;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *page2)
    {
        if (page2->objectName().isEmpty())
            page2->setObjectName(QString::fromUtf8("page2"));
        page2->resize(800, 600);
        centralwidget = new QWidget(page2);
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
        file2input->setGeometry(QRect(70, 120, 371, 41));
        label_2 = new QLabel(widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 120, 61, 41));
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 40, 61, 41));
        file1input = new QLineEdit(widget);
        file1input->setObjectName(QString::fromUtf8("file1input"));
        file1input->setGeometry(QRect(70, 40, 371, 41));
        file1but = new QPushButton(widget);
        file1but->setObjectName(QString::fromUtf8("file1but"));
        file1but->setGeometry(QRect(470, 50, 101, 31));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/png/img/choisefile.png"), QSize(), QIcon::Normal, QIcon::Off);
        file1but->setIcon(icon);
        file2but = new QPushButton(widget);
        file2but->setObjectName(QString::fromUtf8("file2but"));
        file2but->setGeometry(QRect(470, 120, 101, 31));
        file2but->setIcon(icon);
        startcup = new QPushButton(widget);
        startcup->setObjectName(QString::fromUtf8("startcup"));
        startcup->setGeometry(QRect(260, 180, 151, 31));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/png/img/1.png"), QSize(), QIcon::Normal, QIcon::Off);
        startcup->setIcon(icon1);
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        stackedWidget->addWidget(page_2);
        page2->setCentralWidget(centralwidget);
        menubar = new QMenuBar(page2);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 29));
        menu_PSI = new QMenu(menubar);
        menu_PSI->setObjectName(QString::fromUtf8("menu_PSI"));
        page2->setMenuBar(menubar);
        statusbar = new QStatusBar(page2);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        page2->setStatusBar(statusbar);

        menubar->addAction(menu_PSI->menuAction());

        retranslateUi(page2);

        QMetaObject::connectSlotsByName(page2);
    } // setupUi

    void retranslateUi(QMainWindow *page2)
    {
        page2->setWindowTitle(QApplication::translate("page2", "MainWindow", nullptr));
        label_2->setText(QApplication::translate("page2", "\346\226\207\344\273\2662\357\274\232", nullptr));
        label->setText(QApplication::translate("page2", "\346\226\207\344\273\2661\357\274\232", nullptr));
        file1but->setText(QApplication::translate("page2", "\351\200\211\346\213\251\346\226\207\344\273\2661", nullptr));
        file2but->setText(QApplication::translate("page2", "\351\200\211\346\213\251\346\226\207\344\273\2662", nullptr));
        startcup->setText(QApplication::translate("page2", "\345\274\200\345\247\213\350\256\241\347\256\227", nullptr));
        menu_PSI->setTitle(QApplication::translate("page2", "\346\234\254\345\234\260PSI", nullptr));
    } // retranslateUi

};

namespace Ui {
    class page2: public Ui_page2 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PAGE2_H
