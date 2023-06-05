/********************************************************************************
** Form generated from reading UI file 'page1.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PAGE1_H
#define UI_PAGE1_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_page1
{
public:
    QWidget *centralwidget;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QLabel *label;
    QWidget *page_2;
    QMenuBar *menubar;
    QMenu *menu;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *page1)
    {
        if (page1->objectName().isEmpty())
            page1->setObjectName(QString::fromUtf8("page1"));
        page1->resize(800, 600);
        centralwidget = new QWidget(page1);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setGeometry(QRect(0, 0, 800, 600));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        label = new QLabel(page);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 0, 620, 600));
        label->setStyleSheet(QString::fromUtf8("padding-top:0px;"));
        label->setTextFormat(Qt::RichText);
        label->setWordWrap(true);
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        stackedWidget->addWidget(page_2);
        page1->setCentralWidget(centralwidget);
        menubar = new QMenuBar(page1);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        menu = new QMenu(menubar);
        menu->setObjectName(QString::fromUtf8("menu"));
        page1->setMenuBar(menubar);
        statusbar = new QStatusBar(page1);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        page1->setStatusBar(statusbar);

        menubar->addAction(menu->menuAction());

        retranslateUi(page1);

        QMetaObject::connectSlotsByName(page1);
    } // setupUi

    void retranslateUi(QMainWindow *page1)
    {
        page1->setWindowTitle(QApplication::translate("page1", "MainWindow", nullptr));
        label->setText(QApplication::translate("page1", "<html><head/><body><p><span style=\" font-size:14pt; color:#0055ff;\">PSI(private set intersection):</span><span style=\" font-size:12pt;\">\351\232\220\347\247\201\351\233\206\345\220\210\346\261\202\344\272\244\346\230\257\345\256\211\345\205\250\345\244\232\346\226\271\350\256\241\347\256\227\344\270\255\347\232\204\344\270\200\344\270\252\345\237\272\347\241\200\345\215\217\350\256\256\357\274\214\344\270\244\346\226\271PSI\345\205\201\350\256\270\345\220\204\350\207\252\346\214\201\346\234\211\344\270\200\351\232\220\347\247\201\351\233\206\345\220\210\347\232\204\347\224\250\346\210\267\345\256\211\345\205\250\350\256\241\347\256\227\344\270\244\351\233\206\345\220\210\347\232\204\344\272\244\351\233\206\357\274\214\350\276\223\345\207\272\347\273\223\346\236\234\351\231\244\344\272\244\351\233\206\345\244\226\344\270\215\346\263\204\351\234\262\344\273\273\344\275\225\344\277\241\346\201\257\343\200\202 \346\234\254\351\241\271\347\233\256\345\256\236\347\216\260\347\232\204PSI\345\215\217\350\256\256\346"
                        "\235\245\346\272\220\344\272\216\350\256\272\346\226\207</span><a href=\"https://eprint.iacr.org/2020/729.pdf\"><span style=\" font-size:12pt; text-decoration: underline; color:#0000ff;\">\343\200\212Private Set Intersection in the Internet Setting From Lightweight Oblivious PRF\343\200\213</span></a><span style=\" font-size:12pt;\">\345\260\206\345\205\266\344\270\255\347\232\204unordered_map\346\233\277\346\215\242\344\270\272bloom filter\344\275\277\345\205\266\346\225\210\347\216\207\350\277\233\344\270\200\346\255\245\346\217\220\345\215\207\357\274\214\345\257\2712^20\345\244\247\345\260\217\347\232\204\351\233\206\345\220\210\346\261\202\344\272\244\351\233\206\351\234\200\350\246\20110s\345\267\246\345\217\263\357\274\210ubuntu 2\346\240\270\357\274\2144G\357\274\211\343\200\202 </span></p><p align=\"justify\"><span style=\" font-size:14pt; color:#00aaff;\">PIR(Private Information Retrieval):</span><span style=\" font-size:12pt;\">\345\214\277\350\270\252\346\237\245\350\257\242\357\274\210\344\271\237"
                        "\347\247\260\344\270\272\351\232\220\347\247\201\344\277\241\346\201\257\346\243\200\347\264\242\357\274\211\346\230\257\345\256\211\345\205\250\345\244\232\346\226\271\350\256\241\347\256\227\344\270\255\351\235\236\345\270\270\345\256\236\347\224\250\347\232\204\344\270\200\351\227\250\346\212\200\346\234\257\344\270\216\345\272\224\347\224\250\357\274\214\345\217\257\344\273\245\347\224\250\346\235\245\344\277\235\346\212\244\347\224\250\346\210\267\347\232\204\346\237\245\350\257\242\351\232\220\347\247\201\357\274\214\350\277\233\350\200\214\344\271\237\345\217\257\344\273\245\344\277\235\346\212\244\347\224\250\346\210\267\347\232\204\346\237\245\350\257\242\347\273\223\346\236\234\343\200\202\345\205\266\347\233\256\346\240\207\346\230\257\344\277\235\350\257\201\347\224\250\346\210\267\345\220\221\346\225\260\346\215\256\346\272\220\346\226\271\346\217\220\344\272\244\346\237\245\350\257\242\350\257\267\346\261\202\346\227\266\357\274\214\345\234\250\346\237\245\350\257\242\344\277\241\346\201\257\344\270"
                        "\215\350\242\253\346\204\237\347\237\245\344\270\216\346\263\204\346\274\217\347\232\204\345\211\215\346\217\220\344\270\213\345\256\214\346\210\220\346\237\245\350\257\242\343\200\202\345\215\263\345\257\271\344\272\216\346\225\260\346\215\256\346\272\220\346\226\271\346\235\245\350\257\264\357\274\214\345\217\252\347\237\245\351\201\223\346\234\211\346\237\245\350\257\242\345\210\260\346\235\245\357\274\214\344\275\206\346\230\257\344\270\215\347\237\245\351\201\223\347\234\237\346\255\243\347\232\204\346\237\245\350\257\242\346\235\241\344\273\266\343\200\201\344\271\237\345\260\261\344\270\215\347\237\245\351\201\223\345\257\271\346\226\271\346\237\245\344\272\206\344\273\200\344\271\210\343\200\202\346\234\254\351\241\271\347\233\256\345\256\236\347\216\260\344\273\243\347\240\201\346\235\245\346\272\220\344\272\216\350\256\272\346\226\207</span><a href=\"https://eprint.iacr.org/2018/1168.pdf\"><span style=\" font-size:12pt; text-decoration: underline; color:#0000ff;\">\343\200\212PIR with compressed quer"
                        "ies and amortized query processing\343\200\213</span></a><span style=\" font-size:12pt;\">\357\274\214\350\277\231\346\230\257\344\270\200\344\270\252\345\237\272\344\272\216\345\220\214\346\200\201\345\212\240\345\257\206\345\256\236\347\216\260\347\232\204\345\214\277\350\270\252\346\237\245\350\257\242\346\226\271\346\241\210\357\274\214\350\257\245\351\241\271\347\233\256\345\256\236\347\216\260\344\273\216\346\226\207\344\273\266\350\257\273\345\217\226\346\225\260\346\215\256\346\236\204\345\273\272\346\225\260\346\215\256\345\272\223\345\206\215\346\237\245\350\257\242\357\274\214\345\257\271\344\272\21610-1000000\347\232\204\346\237\245\350\257\242\357\274\214\345\274\200\351\224\200\346\227\266\351\227\264\345\244\247\347\272\246\344\270\2727s\357\274\210ubuntu 2\346\240\270\357\274\2144G\357\274\211\343\200\202 </span></p></body></html>", nullptr));
        menu->setTitle(QApplication::translate("page1", "\351\241\271\347\233\256\344\273\213\347\273\215", nullptr));
    } // retranslateUi

};

namespace Ui {
    class page1: public Ui_page1 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PAGE1_H
