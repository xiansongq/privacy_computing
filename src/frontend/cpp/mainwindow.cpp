#include "../include/mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include "QString"
#include "QLabel"
//#include "../include/page1.h"
//#include "../include/page2.h"
//#include "../include/page3.h"
//#include "../include/page4.h"
//#include "../include/page5.h"
#include "../include/pages.h"
//#include "../../qtapi/psi_cm20.hpp"
#include "QTextBrowser"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->widget1->setStyleSheet("#widget1 { border: 2px solid black; }");

    setFixedSize(this->width(),this->height());  //禁止窗口最大化和随意改变窗口的大小
    ui->stackedWidget->addWidget(new page1);
    ui->stackedWidget->addWidget(new page8);
    ui->stackedWidget->addWidget(new page9);
    ui->stackedWidget->addWidget(new page10);
    ui->stackedWidget->addWidget(new page2);
    ui->stackedWidget->addWidget(new page3);
    ui->stackedWidget->addWidget(new page6);
    ui->stackedWidget->addWidget(new page7);
    ui->stackedWidget->addWidget(new page4);
    ui->stackedWidget->addWidget(new page5);

    ui->stackedWidget->setCurrentIndex(2);

    resizeStackedWidgets(ui->stackedWidget);

}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column) {
    QString text = item->text(column);

    QString parentMenuText = "";
    QTreeWidgetItem *parentItem = item->parent(); // 获取父节点

    if (parentItem != nullptr) {
        parentMenuText = parentItem->text(column);
    }
     qDebug()<<parentMenuText;
    QString ancestorMenuText = "";
    QTreeWidgetItem *ancestorItem = item;
    while (ancestorItem->parent() != nullptr) // 循环获取祖先节点
    {
        ancestorItem = ancestorItem->parent();
        ancestorMenuText = ancestorItem->text(column) + " - " + ancestorMenuText; // 按照层级结构拼接祖先节点的文本
    }
    //项目介绍页面
    if(parentMenuText.isEmpty()&& text=="项目介绍"){
        ui->stackedWidget->setCurrentIndex(2);
        resizeStackedWidgets(ui->stackedWidget);
    }
    else if (parentMenuText == "项目介绍" &&(text=="项目介绍"||text.isEmpty())) {
        ui->stackedWidget->setCurrentIndex(2);
        resizeStackedWidgets(ui->stackedWidget);


    }
        else if (parentMenuText == "项目介绍" && text == "PSI") {
        ui->stackedWidget->setCurrentIndex(3);
        resizeStackedWidgets(ui->stackedWidget);

    } else if (parentMenuText == "项目介绍" && text == "PSU") {
        ui->stackedWidget->setCurrentIndex(4);
        resizeStackedWidgets(ui->stackedWidget);
    }else if (parentMenuText == "项目介绍" && text == "PIR") {
        ui->stackedWidget->setCurrentIndex(5);
        resizeStackedWidgets(ui->stackedWidget);

    }

//PSI 本地计算页面
    else if (parentMenuText == "PSI" && text == "本地求交") {
        ui->stackedWidget->setCurrentIndex(6);
        resizeStackedWidgets(ui->stackedWidget);
        //PSI 联机计算页面
    } else if (parentMenuText == "PSI" && text == "联机求交") {
        ui->stackedWidget->setCurrentIndex(7);
        resizeStackedWidgets(ui->stackedWidget);
    }else if (parentMenuText == "PSU" && text == "本地求并") {
        ui->stackedWidget->setCurrentIndex(8);
        resizeStackedWidgets(ui->stackedWidget);
        //PSI 联机计算页面
    } else if (parentMenuText == "PSU" && text == "联机求并") {
        ui->stackedWidget->setCurrentIndex(9);
        resizeStackedWidgets(ui->stackedWidget);
    }

        // PIR 索引查询 本地查询页面
    else if (parentMenuText == "PIR索引查询" && text == "本地查询") {
        ui->stackedWidget->setCurrentIndex(10);
    }
        //PIR 索引查询 联机查询页面
    else if (parentMenuText == "PIR索引查询" && text == "联机查询") {
        ui->stackedWidget->setCurrentIndex(11);
    }
}


void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    //ui->stackedWidget -> setGeometry(rect());
    resizeStackedWidgets(ui->stackedWidget);
}

void MainWindow::resizeStackedWidgets(QStackedWidget *stackedWidget) {
    QSize size = stackedWidget->size();
    for (int i = 0; i < stackedWidget->count(); i++) {
        QWidget *widget = stackedWidget->widget(i);
        widget->resize(size);
    }
}
