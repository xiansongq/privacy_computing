//
// Created by 17579 on 2023/5/30.
//
#include "../include/pages.h"
#include "../../qtapi/psi_factory.cpp"
#include "../../qtapi/psu_factory.cpp"
#include "../../qtapi/sealpir.cpp"
#include "ui_page1.h"
#include "ui_page2.h"
#include "ui_page3.h"
#include "ui_page4.h"
#include "ui_page5.h"
#include "ui_page6.h"
#include "ui_page7.h"
#include "ui_page8.h"
#include "ui_page9.h"
#include "ui_page10.h"

page1::page1(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::page1) {
    ui->setupUi(this);
    //ui->label->setMargin(0);
   // ui->label->setGeometry(0, 0, 620, 390);
  //  ui->label->setText(
   //         "<html><head/><body><p><span style=\" font-size:14pt; color:#0055ff;\">PSI(private set intersection):</span><span style=\" font-size:12pt;\">隐私集合求交是安全多方计算中的一个基础协议，两方PSI允许各自持有一隐私集合的用户安全计算两集合的交集，输出结果除交集外不泄露任何信息。 本项目实现的PSI协议来源于论文</span><a href=\"https://eprint.iacr.org/2020/729.pdf\"><span style=\" font-size:12pt; text-decoration: underline; color:#0000ff;\">《Private Set Intersection in the Internet Setting From Lightweight Oblivious PRF》</span></a><span style=\" font-size:12pt;\">将其中的unordered_map替换为bloom filter使其效率进一步提升，对2^20大小的集合求交集需要10s左右（ubuntu 2核，4G）。 </span></p><p align=\"justify\"><span style=\" font-size:14pt; color:#0055ff;\">PIR(Private Information Retrieval):</span><span style=\" font-size:12pt;\">匿踪查询（也称为隐私信息检索）是安全多方计算中非常实用的一门技术与应用，可以用来保护用户的查询隐私，进而也可以保护用户的查询结果。其目标是保证用户向数据源方提交查询请求时，在查询信息不被感知与泄漏的前提下完成查询。即对于数据源方来说，只知道有查询到来，但是不知道真正的查询条件、也就不知道对方查了什么。本项目实现代码来源于论文</span><a href=\"https://eprint.iacr.org/2018/1168.pdf\"><span style=\" font-size:12pt; text-decoration: underline; color:#0000ff;\">《PIR with compressed queries and amortized query processing》</span></a><span style=\" font-size:12pt;\">，这是一个基于同态加密实现的匿踪查询方案，该项目实现从文件读取数据构建数据库再查询，对于10-1000000的查询，开销时间大约为7s（ubuntu 2核，4G）。 </span></p></body></html>");
//    ui->label->setText("123");
//    ui->label ->hide();
//    label1->setText("123");
//    label1->setGeometry(100, 100, 620, 210);
    // 连接菜单项的信号到槽函数

}

page1::~page1() {
    delete ui;
}


QString path1;
QString path2;

page2::page2(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::page2) {
    ui->setupUi(this);
    ui->label->setFont(QFont("Arial", 14));
    ui->label_2->setFont(QFont("Arial", 14));
//    QIcon icon;
//    icon.addFile(":/png/img/file.png");
//    ui->file1but->setIcon(QIcon(":/png/img/save.png"));
    textBrowser = new QTextBrowser(ui->stackedWidget);
    textBrowser->hide();
    textBrowser->setGeometry(0, 230, 620, 200);         // 设置textbrowser在父窗口中的位置和大小
    savefile = new QPushButton(ui->stackedWidget);
    savefile->setText("保存文件");
    savefile->setStyleSheet("color: blue; font-size: 24px;");
    savefile->setIcon(QIcon(":/png/img/save.png"));
    savefile->setGeometry(280, 430, 150, 42);
    savefile->hide();
    speedlabel = new QLabel(ui->stackedWidget);
    speedlabel->setGeometry(10, 210, 550, 20);
    speedlabel->setStyleSheet("QLabel{color:red;}");
    speedlabel->hide();
    connect(savefile, &QPushButton::clicked, this, &page2::on_savefile_clicked);
}

page2::~page2() {
    delete ui;
}

void page2::on_file1but_clicked() {
    QString path = QFileDialog::getOpenFileName(
            this,
            "open",
            "../",
            "Text files (*.txt)"
    );

    if (path.isEmpty() == false) {
        ui->file1input->setText(path);
        path1 = path;
        //不读取文件内容

    }
}

void page2::on_file2but_clicked() {
    QString path = QFileDialog::getOpenFileName(
            this,
            "open",
            "../",
            "Text files (*.txt)"
    );

    if (path.isEmpty() == false) {
        ui->file2input->setText(path);
        path2 = path;
        //不读取文件内容

    }
}

void page2::on_startcup_clicked() {
    if (page2::type.isEmpty()) {
        QMessageBox::warning(this, "错误", "请先在左上角选择具体的协议！");
        return;
    }
    qDebug() << page2::type;
    if (path1.isEmpty() == true || path2.isEmpty() == true) {
        QMessageBox::warning(this, "waring", "请选择文件");
        //::path.clear();
    } else {
        loading1 = new Loading(this);
        loading1->start();
        loading1->setGeometry(240, 180, 200, 200);
        loading1->show();
        // 在子线程中执行loadData函数，并获取数据结果
        QFuture <Message> result = QtConcurrent::run([&]() {
            return PSIFactory::local_psi(path1.toStdString(), path2.toStdString(), page2::type.toStdString());
        });
        // 创建一个QFutureWatcher对象，并连接其信号和槽函数
        QFutureWatcher <Message> *watcher = new QFutureWatcher<Message>;
        QObject::connect(watcher, &QFutureWatcher<Message>::finished, [=]() {
            // 异步操作完成，停止动画并获取结果
            Message ans = watcher->result();

            //关闭对话
            loading1->hide();
            // 删除并释放watcher对象
            QObject::disconnect(watcher, nullptr, nullptr, nullptr);
            watcher->deleteLater();
            //后续将数据输出到文本框中
            if (ans.code != 200) {
                //将textBrowser中的文本清空
                textBrowser->clear();
                speedlabel->clear();
                QMessageBox::warning(this, "waring", QString::fromStdString(ans.msg), QMessageBox::Yes);
            } else {
                //将textBrowser中的文本清空
                textBrowser->clear();
                speedlabel->clear();
                qDebug() << ans.data.size();
                for (auto a: ans.data) {
                    textBrowser->append(QString::fromStdString(a));
                }
                textBrowser->show();
                speedlabel->setText(
                        "接收方时间开销：" + QString::fromStdString(to_string(ans.receiver_time)) + "ms" + "  " +
                        "发送方时间开销：" + QString::fromStdString(to_string(ans.sender_time)) + "ms");
                speedlabel->show();
                savefile->show();

            }
        });

        watcher->setFuture(result);

    }


}

void page2::on_savefile_clicked() {

    QString str = textBrowser->toPlainText();
    if (str.isEmpty() == false) {
        QString savepath = QFileDialog::getSaveFileName(this, "save", "../", "TXT files(*.txt)");

        if (savepath.isEmpty() == false) {
            QFileInfo fileInfo(savepath);
            QString filePath = fileInfo.filePath();
            if (!filePath.endsWith(".txt", Qt::CaseInsensitive)) {
                filePath += ".txt";
            }
            QFile file;
            file.setFileName(filePath);
            bool isok = file.open(QIODevice::WriteOnly);
            if (isok == true) {
                QMessageBox::StandardButton result = QMessageBox::information(this, "保存", "是否保存文件",
                                                                              QMessageBox::Yes | QMessageBox::No);
                switch (result) {
                    case QMessageBox::Yes:
                        if (isok == true) {
                            QString str = textBrowser->toPlainText();
                            file.write(str.toUtf8());

                        }
                        file.close();
                        QMessageBox::information(this, "提示", "保存成功！");
                        break;
                    case QMessageBox::No:
                        qDebug() << "NO";
                        break;
                    default:
                        break;
                }

            } else {
                QMessageBox::warning(this, "错误", "无法打开文件进行写入！");
            }

        }
    }

};


QString path;

page3::page3(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::page3) {
    ui->setupUi(this);
    ui->label->setFont(QFont("Arial", 14));
    ui->label_2->setFont(QFont("Arial", 14));
    ui->label_3->setText("提示：两方通过网络通信执行PSI协议，首先启动接收方网络通信，输出结果只有接收方可见。");
    textBrowser = new QTextBrowser(ui->stackedWidget);
    textBrowser->setGeometry(0, 230, 620, 200);         // 设置textbrowser在父窗口中的位置和大小
    textBrowser->hide();
    savefile = new QPushButton(ui->stackedWidget);
    savefile->setText("保存文件");
    savefile->setStyleSheet("color: blue; font-size: 24px;");
    savefile->setIcon(QIcon(":/png/img/save.png"));
    savefile->setGeometry(280, 430, 150, 42);
    savefile->hide();
    savefile->setObjectName("savefile");
    speedlabel = new QLabel(ui->stackedWidget);
    speedlabel->setGeometry(200, 210, 271, 20);
    speedlabel->setStyleSheet("QLabel{color:red;}");
    speedlabel->hide();
    //绑定保存文件按钮
    connect(savefile, &QPushButton::clicked, this, &page3::on_savefile_clicked);
}

page3::~page3() {
    delete ui;
}

void page3::on_choisefile_clicked() {
    QString path = QFileDialog::getOpenFileName(
            this,
            "open",
            "../",
            "Text files (*.txt)"
    );

    if (path.isEmpty() == false) {
        ui->filepath->setText(path);
        ::path = path;
        //不读取文件内容
    }
}

void page3::on_startcpu_clicked() {
    if (page3::type.isEmpty()) {
        QMessageBox::warning(this, "错误", "请先在左上角选择具体的协议！");
        return;
    }
    // 获取基本的参数
    QString ip, port;
    std::string isflag = "0";
    ip = ui->ip->text();
    port = ui->port->text();
    qDebug() << ui->isserver->isChecked();
    if (ui->isserver->isChecked()) isflag = "1";
    else isflag = "0";

    // 判断这些参数 是否为空
    if (ip.isEmpty() || port.isEmpty()) {
        QMessageBox::warning(this, "waring", "参数不能为空！");
    } else if (::path.isEmpty()) {
        QMessageBox::warning(this, "waring", "请选择文件！");
    } else {
        loading1 = new Loading(this);
        loading1->start();
        loading1->setGeometry(240, 180, 200, 200);
        loading1->show();
        // 在子线程中执行loadData函数，并获取数据结果
        QFuture <Message> result = QtConcurrent::run([&]() {
            std::cout << "isserverflag: " << isflag << std::endl;
            return PSIFactory::remote_psi(ip.toStdString(), port.toStdString(), ::path.toStdString(), isflag,
                                          page3::type.toStdString());
        });
        // 创建一个QFutureWatcher对象，并连接其信号和槽函数
        QFutureWatcher <Message> *watcher = new QFutureWatcher<Message>;
        QObject::connect(watcher, &QFutureWatcher<Message>::finished, [=]() {
            // 异步操作完成，停止动画并获取结果
            Message ans = watcher->result();

            //关闭对话
            loading1->hide();
            // 删除并释放watcher对象
            QObject::disconnect(watcher, nullptr, nullptr, nullptr);
            watcher->deleteLater();
            //后续对将数据输出到文本框中
            if (ans.code != 200) {
                //将textBrowser中的文本清空
                textBrowser->clear();
                speedlabel->clear();
                QMessageBox::warning(this, "waring", QString::fromStdString(ans.msg));
            } else {
                if (isflag == "1") {
                    //将textBrowser中的文本清空
                    textBrowser->clear();
                    speedlabel->clear();
                    for (auto a: ans.data) {
                        // text+=QString::fromStdString(a)+"\n";
                        textBrowser->append(QString::fromStdString(a));
                    }
                    //textBrowser->setText(text);
                    speedlabel->setText("时间开销：" + QString::fromStdString(to_string(ans.receiver_time)) + "ms");
                    speedlabel->show();
                    textBrowser->show();
                    savefile->show();
                } else {
                    textBrowser->hide();
                    savefile->hide();
                    speedlabel->clear();
                    speedlabel->setText("时间开销：" + QString::fromStdString(to_string(ans.sender_time)) + "ms");
                    speedlabel->show();
                }


            }
        });

        watcher->setFuture(result);
    }


}

void page3::on_savefile_clicked() {
    QString str = textBrowser->toPlainText();
    if (str.isEmpty() == false) {
        QString savepath = QFileDialog::getSaveFileName(this, "save", "../", "TXT files(*.txt)");

        if (savepath.isEmpty() == false) {
            QFileInfo fileInfo(savepath);
            QString filePath = fileInfo.filePath();
            if (!filePath.endsWith(".txt", Qt::CaseInsensitive)) {
                filePath += ".txt";
            }
            QFile file;
            file.setFileName(filePath);
            bool isok = file.open(QIODevice::WriteOnly);
            if (isok == true) {
                QMessageBox::StandardButton result = QMessageBox::information(this, "保存", "是否保存文件",
                                                                              QMessageBox::Yes | QMessageBox::No);
                switch (result) {
                    case QMessageBox::Yes:
                        if (isok == true) {
                            QString str = textBrowser->toPlainText();
                            file.write(str.toUtf8());

                        }
                        file.close();
                        QMessageBox::information(this, "提示", "保存成功！");
                        break;
                    case QMessageBox::No:
                        qDebug() << "NO";
                        break;
                    default:
                        break;
                }

            } else {
                QMessageBox::warning(this, "错误", "无法打开文件进行写入！");
            }

        }
    }
}

QString path11;
QString path21;

page4::page4(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::page4) {
    ui->setupUi(this);
    textBrowser = new QTextBrowser(ui->stackedWidget);
    textBrowser->setGeometry(0, 230, 620, 200);         // 设置textbrowser在父窗口中的位置和大小
    textBrowser->hide();
    savefile = new QPushButton(ui->stackedWidget);
    savefile->setText("保存文件");
    savefile->setStyleSheet("color: blue; font-size: 24px;");
    savefile->setIcon(QIcon(":/png/img/save.png"));
    savefile->setGeometry(280, 430, 150, 42);
    savefile->hide();
    speedlabel = new QLabel(ui->stackedWidget);
    speedlabel->setGeometry(200, 210, 271, 20);
    speedlabel->setStyleSheet("QLabel{color:red;}");
    speedlabel->hide();
    connect(savefile, &QPushButton::clicked, this, &page4::handleSaveFile);

}

page4::~page4() {
    delete ui;
}

void page4::on_file1but_clicked() {
    QString path = QFileDialog::getOpenFileName(
            this,
            "open",
            "../",
            "Text files (*.txt)"
    );

    if (path.isEmpty() == false) {
        ui->file1input->setText(path);
        path11 = path;
        //不读取文件内容

    }
}

void page4::on_file2but_clicked() {
    QString path = QFileDialog::getOpenFileName(
            this,
            "open",
            "../",
            "Text files (*.txt)"
    );

    if (path.isEmpty() == false) {
        ui->file2input->setText(path);
        path21 = path;
        //不读取文件内容

    }
}

void page4::on_startcup_clicked() {
    //首先校验文件是否选择
    if (path11.isEmpty() == true || path21.isEmpty() == true) {
        QMessageBox::warning(this, "waring", "请选择文件");
    } else {
        loading1 = new Loading(this);
        loading1->start();
        loading1->setGeometry(240, 180, 200, 200);
        loading1->show();
        // 在子线程中执行loadData函数，并获取数据结果
        QFuture <Message> result = QtConcurrent::run([&]() {
            return PIR::localPIR(path11.toStdString(), path21.toStdString());
        });
        // 创建一个QFutureWatcher对象，并连接其信号和槽函数
        QFutureWatcher <Message> *watcher = new QFutureWatcher<Message>;
        QObject::connect(watcher, &QFutureWatcher<Message>::finished, [=]() {
            // 异步操作完成，停止动画并获取结果
            Message ans = watcher->result();

            //关闭对话
            loading1->hide();
            // 删除并释放watcher对象
            QObject::disconnect(watcher, nullptr, nullptr, nullptr);
            watcher->deleteLater();
            //后续将数据输出到文本框中
            if (ans.code != 200) {
                //将textBrowser中的文本清空
                speedlabel->clear();
                textBrowser->clear();
                QMessageBox::warning(NULL, "waring", QString::fromStdString(ans.msg), QMessageBox::Yes);
            } else {
                //将textBrowser中的文本清空
                textBrowser->clear();
                speedlabel->clear();
                for (auto a: ans.pirdata) {
                    textBrowser->append(QString::fromStdString(a));
                }
                speedlabel->setText("时间开销：" + QString::fromStdString(to_string(ans.sender_time)) + "ms");
                speedlabel->show();
                textBrowser->show();
                savefile->show();
            }
        });

        watcher->setFuture(result);
    }
}

void page4::handleSaveFile() {
    QString str = textBrowser->toPlainText();
    if (str.isEmpty() == false) {
        QString savepath = QFileDialog::getSaveFileName(this, "save", "../", "TXT files(*.txt)");

        if (savepath.isEmpty() == false) {
            QFileInfo fileInfo(savepath);
            QString filePath = fileInfo.filePath();
            if (!filePath.endsWith(".txt", Qt::CaseInsensitive)) {
                filePath += ".txt";
            }
            QFile file;
            file.setFileName(filePath);
            bool isok = file.open(QIODevice::WriteOnly);
            if (isok == true) {
                QMessageBox::StandardButton result = QMessageBox::information(this, "保存", "是否保存文件",
                                                                              QMessageBox::Yes | QMessageBox::No);
                switch (result) {
                    case QMessageBox::Yes:
                        if (isok == true) {
                            QString str = textBrowser->toPlainText();
                            file.write(str.toUtf8());

                        }
                        file.close();
                        QMessageBox::information(this, "提示", "保存成功！");
                        break;
                    case QMessageBox::No:
                        qDebug() << "NO";
                        break;
                    default:
                        break;
                }

            } else {
                QMessageBox::warning(this, "错误", "无法打开文件进行写入！");
            }

        }
    }

}

QString pirpath;

page5::page5(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::page5) {
    ui->setupUi(this);
    ui->label_3->setText("提示：通过网络通信执行PIR查询，首先要启动数据库提供方网络通信，查询结果只有查询方可间。");
    textBrowser = new QTextBrowser(ui->stackedWidget);
    textBrowser->setGeometry(0, 230, 620, 200);         // 设置textbrowser在父窗口中的位置和大小
    textBrowser->hide();
    savefile = new QPushButton(ui->stackedWidget);
    savefile->setText("保存文件");
    savefile->setStyleSheet("color: blue; font-size: 24px;");
    savefile->setIcon(QIcon(":/png/img/save.png"));
    savefile->setGeometry(280, 430, 150, 42);
    savefile->hide();
    savefile->setObjectName("savefile");
    speedlabel = new QLabel(ui->stackedWidget);
    speedlabel->setGeometry(200, 210, 271, 20);
    speedlabel->setStyleSheet("QLabel{color:red;}");
    speedlabel->hide();
    //绑定保存文件按钮
    connect(savefile, &QPushButton::clicked, this, &page5::on_savefile_clicked);
}

page5::~page5() {
    delete ui;
}

void page5::on_choisefile_clicked() {
    QString path = QFileDialog::getOpenFileName(
            this,
            "open",
            "../",
            "Text files (*.txt)"
    );

    if (path.isEmpty() == false) {
        ui->filepath->setText(path);
        pirpath = path;
        //不读取文件内容
    }
}

void page5::on_startcpu_clicked() {
    // 获取基本的参数
    QString ip, port, isclient;
    ip = ui->ip->text();
    port = ui->port->text();
    if (ui->isserver->isChecked()) isclient = "1";
    else isclient = "0";

    //首先校验文件是否选择
    if (pirpath.isEmpty() == true || ip.isEmpty() == true || port.isEmpty() == true) {
        QMessageBox::warning(this, "waring", "请输入参数", QMessageBox::Yes);
    } else {
        loading1 = new Loading(this);
        loading1->start();
        loading1->setGeometry(240, 180, 200, 200);
        loading1->show();
        // 在子线程中执行loadData函数，并获取数据结果
        QFuture <Message> result = QtConcurrent::run([&]() {
            return PIR::RemotePIR(ip.toStdString(), port.toStdString(), isclient.toStdString(), pirpath.toStdString());
        });
        // 创建一个QFutureWatcher对象，并连接其信号和槽函数
        QFutureWatcher <Message> *watcher = new QFutureWatcher<Message>;
        QObject::connect(watcher, &QFutureWatcher<Message>::finished, [=]() {
            // 异步操作完成，停止动画并获取结果
            Message ans = watcher->result();

            //关闭对话
            loading1->hide();
            // 删除并释放watcher对象
            QObject::disconnect(watcher, nullptr, nullptr, nullptr);
            watcher->deleteLater();
            //后续对将数据输出到文本框中
            if (ans.code != 200) {
                //将textBrowser中的文本清空
                textBrowser->clear();
                speedlabel->clear();
                QMessageBox::warning(this, "waring", QString::fromStdString(ans.msg), QMessageBox::Yes);
            } else {
                if (isclient == "1") {
                    //将textBrowser中的文本清空
                    textBrowser->clear();
                    speedlabel->clear();
                    for (auto a: ans.pirdata) {
                        textBrowser->append(QString::fromStdString(a));
                        //std::cout<<Block::BlockToInt64(a)<<std::endl;
                    }
                    speedlabel->setText("时间开销：" + QString::fromStdString(to_string(ans.receiver_time)) + "ms");
                    speedlabel->show();
                    textBrowser->show();
                    savefile->show();
                } else {
                    textBrowser->hide();
                    savefile->hide();
                    speedlabel->clear();
                    speedlabel->setText("时间开销：" + QString::fromStdString(to_string(ans.sender_time)) + "ms");
                    speedlabel->show();
                }

            }
        });

        watcher->setFuture(result);
    }
}

void page5::on_savefile_clicked() {

    QString str = textBrowser->toPlainText();
    if (str.isEmpty() == false) {
        QString savepath = QFileDialog::getSaveFileName(this, "save", "../", "TXT files(*.txt)");

        if (savepath.isEmpty() == false) {
            QFileInfo fileInfo(savepath);
            QString filePath = fileInfo.filePath();
            if (!filePath.endsWith(".txt", Qt::CaseInsensitive)) {
                filePath += ".txt";
            }
            QFile file;
            file.setFileName(filePath);
            bool isok = file.open(QIODevice::WriteOnly);
            if (isok == true) {
                QMessageBox::StandardButton result = QMessageBox::information(this, "保存", "是否保存文件",
                                                                              QMessageBox::Yes | QMessageBox::No);
                switch (result) {
                    case QMessageBox::Yes:
                        if (isok == true) {
                            QString str = textBrowser->toPlainText();
                            file.write(str.toUtf8());

                        }
                        file.close();
                        QMessageBox::information(this, "提示", "保存成功！");
                        break;
                    case QMessageBox::No:
                        qDebug() << "NO";
                        break;
                    default:
                        break;
                }

            } else {
                QMessageBox::warning(this, "错误", "无法打开文件进行写入！");
            }

        }
    }

}

void page2::on_actionCM20_triggered() {
    page2::type = "CM20";
    ui->menu_PSI->setTitle(page2::type);

}

void page2::on_actionECDH_triggered() {
    page2::type = "ECDH";
    ui->menu_PSI->setTitle(page2::type);
}

void page3::on_actionCM20_triggered() {
    page3::type = "CM20";
    ui->menuPSI->setTitle(page3::type);

}

void page3::on_actionECDH_triggered() {
    page3::type = "ECDH";
    ui->menuPSI->setTitle(page3::type);
}


QString path61;
QString path62;

page6::page6(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::page6) {
    ui->setupUi(this);
    ui->label_15->setFont(QFont("Arial", 14));
    ui->label_16->setFont(QFont("Arial", 14));
//    QIcon icon;
//    icon.addFile(":/png/img/file.png");
//    ui->file1but->setIcon(QIcon(":/png/img/save.png"));
    textBrowser = new QTextBrowser(ui->stackedWidget);
    textBrowser->hide();
    textBrowser->setGeometry(0, 230, 620, 200);         // 设置textbrowser在父窗口中的位置和大小
    savefile = new QPushButton(ui->stackedWidget);
    savefile->setText("保存文件");
    savefile->setStyleSheet("color: blue; font-size: 24px;");
    savefile->setIcon(QIcon(":/png/img/save.png"));
    savefile->setGeometry(280, 430, 150, 42);
    savefile->hide();
    speedlabel = new QLabel(ui->stackedWidget);
    speedlabel->setGeometry(10, 210, 550, 20);
    speedlabel->setStyleSheet("QLabel{color:red;}");
    speedlabel->hide();
    connect(savefile, &QPushButton::clicked, this, &page6::on_savefile_clicked);
}

page6::~page6() {
    delete ui;
}

void page6::on_file1but_6_clicked() {
    QString path = QFileDialog::getOpenFileName(
            this,
            "open",
            "../",
            "Text files (*.txt)"
    );

    if (path.isEmpty() == false) {
        ui->file1input_6->setText(path);
        path61 = path;
        //不读取文件内容

    }
}

void page6::on_file2but_6_clicked() {
    QString path = QFileDialog::getOpenFileName(
            this,
            "open",
            "../",
            "Text files (*.txt)"
    );

    if (path.isEmpty() == false) {
        ui->file2input_6->setText(path);
        path62 = path;
        //不读取文件内容

    }
}

void page6::on_startcup_6_clicked() {

    page6::type="ecc_mqrpmt";
    if (path61.isEmpty() == true || path62.isEmpty() == true) {
        QMessageBox::warning(this, "waring", "请选择文件");
        //::path.clear();
    } else {
        loading1 = new Loading(this);
        loading1->start();
        loading1->setGeometry(240, 180, 200, 200);
        loading1->show();
        // 在子线程中执行loadData函数，并获取数据结果
        QFuture <Message> result = QtConcurrent::run([&]() {
            return PSUFactory::local_psu(path61.toStdString(), path62.toStdString(), page6::type.toStdString());
        });
        // 创建一个QFutureWatcher对象，并连接其信号和槽函数
        QFutureWatcher <Message> *watcher = new QFutureWatcher<Message>;
        QObject::connect(watcher, &QFutureWatcher<Message>::finished, [=]() {
            // 异步操作完成，停止动画并获取结果
            Message ans = watcher->result();

            //关闭对话
            loading1->hide();
            // 删除并释放watcher对象
            QObject::disconnect(watcher, nullptr, nullptr, nullptr);
            watcher->deleteLater();
            //后续将数据输出到文本框中
            if (ans.code != 200) {
                //将textBrowser中的文本清空
                textBrowser->clear();
                speedlabel->clear();
                QMessageBox::warning(this, "waring", QString::fromStdString(ans.msg), QMessageBox::Yes);
            } else {
                //将textBrowser中的文本清空
                textBrowser->clear();
                speedlabel->clear();
                qDebug() << ans.data.size();
                for (auto a: ans.data) {
                    textBrowser->append(QString::fromStdString(a));
                }
                textBrowser->show();
                speedlabel->setText(
                        "接收方时间开销：" + QString::fromStdString(to_string(ans.receiver_time)) + "ms" + "  " +
                        "发送方时间开销：" + QString::fromStdString(to_string(ans.sender_time)) + "ms");
                speedlabel->show();
                savefile->show();

            }
        });

        watcher->setFuture(result);

    }


}

void page6::on_savefile_clicked() {

    QString str = textBrowser->toPlainText();
    if (str.isEmpty() == false) {
        QString savepath = QFileDialog::getSaveFileName(this, "save", "../", "TXT files(*.txt)");

        if (savepath.isEmpty() == false) {
            QFileInfo fileInfo(savepath);
            QString filePath = fileInfo.filePath();
            if (!filePath.endsWith(".txt", Qt::CaseInsensitive)) {
                filePath += ".txt";
            }
            QFile file;
            file.setFileName(filePath);
            bool isok = file.open(QIODevice::WriteOnly);
            if (isok == true) {
                QMessageBox::StandardButton result = QMessageBox::information(this, "保存", "是否保存文件",
                                                                              QMessageBox::Yes | QMessageBox::No);
                switch (result) {
                    case QMessageBox::Yes:
                        if (isok == true) {
                            QString str = textBrowser->toPlainText();
                            file.write(str.toUtf8());

                        }
                        file.close();
                        QMessageBox::information(this, "提示", "保存成功！");
                        break;
                    case QMessageBox::No:
                        qDebug() << "NO";
                        break;
                    default:
                        break;
                }

            } else {
                QMessageBox::warning(this, "错误", "无法打开文件进行写入！");
            }

        }
    }

};


QString path7;

page7::page7(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::page7) {
    ui->setupUi(this);
    ui->label->setFont(QFont("Arial", 14));
    ui->label_2->setFont(QFont("Arial", 14));
    ui->label_3->setText("提示：两方通过网络通信执行PSI协议，首先启动接收方网络通信，输出结果只有接收方可见。");
    textBrowser = new QTextBrowser(ui->stackedWidget);
    textBrowser->setGeometry(0, 230, 620, 200);         // 设置textbrowser在父窗口中的位置和大小
    textBrowser->hide();
    savefile = new QPushButton(ui->stackedWidget);
    savefile->setText("保存文件");
    savefile->setStyleSheet("color: blue; font-size: 24px;");
    savefile->setIcon(QIcon(":/png/img/save.png"));
    savefile->setGeometry(280, 430, 150, 42);
    savefile->hide();
    savefile->setObjectName("savefile");
    speedlabel = new QLabel(ui->stackedWidget);
    speedlabel->setGeometry(200, 210, 271, 20);
    speedlabel->setStyleSheet("QLabel{color:red;}");
    speedlabel->hide();
    //绑定保存文件按钮
    connect(savefile, &QPushButton::clicked, this, &page7::on_savefile_clicked);
}

page7::~page7() {
    delete ui;
}

void page7::on_choisefile_clicked() {
    QString path = QFileDialog::getOpenFileName(
            this,
            "open",
            "../",
            "Text files (*.txt)"
    );

    if (path.isEmpty() == false) {
        ui->filepath->setText(path);
        ::path = path;
        //不读取文件内容
    }
}

void page7::on_startcpu_clicked() {
    page7::type="ecc_mqrpmt";
    // 获取基本的参数
    QString ip, port;
    std::string isflag = "0";
    ip = ui->ip->text();
    port = ui->port->text();
    qDebug() << ui->isserver->isChecked();
    if (ui->isserver->isChecked()) isflag = "1";
    else isflag = "0";

    // 判断这些参数 是否为空
    if (ip.isEmpty() || port.isEmpty()) {
        QMessageBox::warning(this, "waring", "参数不能为空！");
    } else if (::path.isEmpty()) {
        QMessageBox::warning(this, "waring", "请选择文件！");
    } else {
        loading1 = new Loading(this);
        loading1->start();
        loading1->setGeometry(240, 180, 200, 200);
        loading1->show();
        // 在子线程中执行loadData函数，并获取数据结果
        QFuture <Message> result = QtConcurrent::run([&]() {
            std::cout << "isserverflag: " << isflag << std::endl;
            return PSUFactory::remote_psu(ip.toStdString(), port.toStdString(), ::path.toStdString(), isflag,
                                          page7::type.toStdString());
        });
        // 创建一个QFutureWatcher对象，并连接其信号和槽函数
        QFutureWatcher <Message> *watcher = new QFutureWatcher<Message>;
        QObject::connect(watcher, &QFutureWatcher<Message>::finished, [=]() {
            // 异步操作完成，停止动画并获取结果
            Message ans = watcher->result();

            //关闭对话
            loading1->hide();
            // 删除并释放watcher对象
            QObject::disconnect(watcher, nullptr, nullptr, nullptr);
            watcher->deleteLater();
            //后续对将数据输出到文本框中
            if (ans.code != 200) {
                //将textBrowser中的文本清空
                textBrowser->clear();
                speedlabel->clear();
                QMessageBox::warning(this, "waring", QString::fromStdString(ans.msg));
            } else {
                if (isflag == "1") {
                    //将textBrowser中的文本清空
                    textBrowser->clear();
                    speedlabel->clear();
                    for (auto a: ans.data) {
                        // text+=QString::fromStdString(a)+"\n";
                        textBrowser->append(QString::fromStdString(a));
                    }
                    //textBrowser->setText(text);
                    speedlabel->setText("时间开销：" + QString::fromStdString(to_string(ans.receiver_time)) + "ms");
                    speedlabel->show();
                    textBrowser->show();
                    savefile->show();
                } else {
                    textBrowser->hide();
                    savefile->hide();
                    speedlabel->clear();
                    speedlabel->setText("时间开销：" + QString::fromStdString(to_string(ans.sender_time)) + "ms");
                    speedlabel->show();
                }


            }
        });

        watcher->setFuture(result);
    }


}

void page7::on_savefile_clicked() {
    QString str = textBrowser->toPlainText();
    if (str.isEmpty() == false) {
        QString savepath = QFileDialog::getSaveFileName(this, "save", "../", "TXT files(*.txt)");

        if (savepath.isEmpty() == false) {
            QFileInfo fileInfo(savepath);
            QString filePath = fileInfo.filePath();
            if (!filePath.endsWith(".txt", Qt::CaseInsensitive)) {
                filePath += ".txt";
            }
            QFile file;
            file.setFileName(filePath);
            bool isok = file.open(QIODevice::WriteOnly);
            if (isok == true) {
                QMessageBox::StandardButton result = QMessageBox::information(this, "保存", "是否保存文件",
                                                                              QMessageBox::Yes | QMessageBox::No);
                switch (result) {
                    case QMessageBox::Yes:
                        if (isok == true) {
                            QString str = textBrowser->toPlainText();
                            file.write(str.toUtf8());

                        }
                        file.close();
                        QMessageBox::information(this, "提示", "保存成功！");
                        break;
                    case QMessageBox::No:
                        qDebug() << "NO";
                        break;
                    default:
                        break;
                }

            } else {
                QMessageBox::warning(this, "错误", "无法打开文件进行写入！");
            }

        }
    }
}

page8::page8(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::page8)
{
    ui->setupUi(this);
}

page8::~page8()
{
    delete ui;
}

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
