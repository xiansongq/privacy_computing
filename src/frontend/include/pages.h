//
// Created by 17579 on 2023/5/30.
//

#ifndef PRIVATE_PAGES_H
#define PRIVATE_PAGES_H

#include <QMainWindow>
#include <QTextBrowser>
#include <QPushButton>
#include <QDebug>
#include <QString>
#include<QFileDialog>
#include<QMessageBox>
#include <QProgressDialog>
#include "loading.h"
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent>
#include <functional>

//#include "../include/WorkThread.h"
namespace Ui {
    class page1;

    class page2;

    class page3;

    class page4;

    class page5;
    class page6;
    class page7;
    class page8;
    class page9;
    class page10;
}

class page1 : public QMainWindow {
    Q_OBJECT

public:
    explicit page1(QWidget *parent = nullptr);

    ~page1();

private:
    Ui::page1 *ui;
    QLabel *label1;
};

class page2 : public QMainWindow {
    Q_OBJECT

public:
    explicit page2(QWidget *parent = nullptr);

    ~page2();

private
    slots:
            void on_file1but_clicked();

    void on_file2but_clicked();

    void on_startcup_clicked();

    void on_savefile_clicked();
    void on_actionCM20_triggered();
    void on_actionECDH_triggered();

private:
    Ui::page2 *ui;
    QTextBrowser *textBrowser;
    QPushButton *savefile;
    Loading *loading1;
    QLabel *speedlabel;
    QString type;
};

class page3 : public QMainWindow {
    Q_OBJECT

public:
    explicit page3(QWidget *parent = nullptr);

    ~page3();

private
    slots:
            void on_choisefile_clicked();

    void on_startcpu_clicked();

    void on_savefile_clicked();
    void on_actionCM20_triggered();
    void on_actionECDH_triggered();

private:
    Ui::page3 *ui;
    QTextBrowser *textBrowser;
    QPushButton *savefile;
    Loading *loading1;
    QLabel *speedlabel;
    QString type;
};

class page4 : public QMainWindow {
    Q_OBJECT

public:
    explicit page4(QWidget *parent = nullptr);

    ~page4();

private
    slots:
            void on_file1but_clicked();

    void on_file2but_clicked();

    void on_startcup_clicked();

    void handleSaveFile();

private:
    Ui::page4 *ui;
    QTextBrowser *textBrowser;
    QPushButton *savefile;
    Loading *loading1;
    QLabel *speedlabel;
};

class page5 : public QMainWindow {
    Q_OBJECT

public:
    explicit page5(QWidget *parent = nullptr);

    ~page5();

private
    slots:
            void on_choisefile_clicked();

    void on_startcpu_clicked();

    void on_savefile_clicked();

private:
    Ui::page5 *ui;
    QTextBrowser *textBrowser;
    QPushButton *savefile;
    Loading *loading1;
    QLabel *speedlabel;
};




class page6 : public QMainWindow {
    Q_OBJECT

public:
    explicit page6(QWidget *parent = nullptr);

    ~page6();

private
    slots:
            void on_file1but_6_clicked();

    void on_file2but_6_clicked();

    void on_startcup_6_clicked();

    void on_savefile_clicked();


private:
    Ui::page6 *ui;
    QTextBrowser *textBrowser;
    QPushButton *savefile;
    Loading *loading1;
    QLabel *speedlabel;
    QString type;
};

class page7 : public QMainWindow {
    Q_OBJECT

public:
    explicit page7(QWidget *parent = nullptr);

    ~page7();

private
    slots:
            void on_choisefile_clicked();

    void on_startcpu_clicked();

    void on_savefile_clicked();


private:
    Ui::page7 *ui;
    QTextBrowser *textBrowser;
    QPushButton *savefile;
    Loading *loading1;
    QLabel *speedlabel;
    QString type;
};
class page8 : public QMainWindow
{
    Q_OBJECT

public:
    explicit page8(QWidget *parent = nullptr);
    ~page8();

private:
    Ui::page8 *ui;
};
class page9 : public QMainWindow
{
    Q_OBJECT

public:
    explicit page9(QWidget *parent = nullptr);
    ~page9();

private:
    Ui::page9 *ui;
};
class page10 : public QMainWindow
{
    Q_OBJECT

public:
    explicit page10(QWidget *parent = nullptr);
    ~page10();

private:
    Ui::page10 *ui;
};


#endif //PRIVATE_PAGES_H
