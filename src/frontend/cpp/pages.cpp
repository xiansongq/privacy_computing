//
// Created by 17579 on 2023/5/30.
//
#include "../include/pages.h"
#include "../../qtapi/psi_cm20.cpp"
#include "../../qtapi/sealpir.cpp"
#include "ui_page1.h"
#include "ui_page2.h"
#include "ui_page3.h"
#include "ui_page4.h"
#include "ui_page5.h"

page1::page1(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::page1) {
    ui->setupUi(this);
    ui->label->setMargin(0);

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
    textBrowser = new QTextBrowser(ui->stackedWidget);
    textBrowser->hide();
    textBrowser->setGeometry(0, 250, 620, 200);         // 设置textbrowser在父窗口中的位置和大小
    savefile = new QPushButton(ui->stackedWidget);
    savefile->setText("保存文件");
    savefile->setStyleSheet("color: blue; font-size: 24px;");
    savefile->setIcon(QIcon(":/png/img/save.png"));
    savefile->setGeometry(280, 460, 150, 42);
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
            return PSI::localhostPSI(path1.toStdString(), path2.toStdString());
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
                QMessageBox::warning(this, "waring", QString::fromStdString(ans.msg), QMessageBox::Yes);
            } else {
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
            savepath = savepath + ".txt"; // 文件名加上后缀
            QFile file;
            file.setFileName(savepath);
            bool isok = file.open(QIODevice::WriteOnly);
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
            return PSI::remotepsi(ip.toStdString(), port.toStdString(), ::path.toStdString(), isflag);
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
                QMessageBox::warning(this, "waring", QString::fromStdString(ans.msg));
            } else {
                if (isflag == "1") {
                    QString text = "";
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
            savepath = savepath + ".txt"; // 文件名加上后缀
            QFile file;
            file.setFileName(savepath);
            bool isok = file.open(QIODevice::WriteOnly);
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
    savefile->setGeometry(280, 440, 150, 42);
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
                QMessageBox::warning(NULL, "waring", QString::fromStdString(ans.msg), QMessageBox::Yes);
            } else {
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
            savepath = savepath + ".txt"; // 文件名加上后缀
            QFile file;
            file.setFileName(savepath);
            bool isok = file.open(QIODevice::WriteOnly);
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
        }
    }

}

QString pirpath;

page5::page5(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::page5) {
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
                QMessageBox::warning(this, "waring", QString::fromStdString(ans.msg), QMessageBox::Yes);
            } else {
                if (isclient == "1") {
                    for (auto a: ans.pirdata) {
                        textBrowser->append(QString::fromStdString(a));
                        //std::cout<<Block::BlockToInt64(a)<<std::endl;
                    }
                    speedlabel->setText("时间开销：" + QString::fromStdString(to_string(ans.receiver_time)) + "ms");
                    speedlabel->show();
                    textBrowser->show();
                    savefile->show();
                } else {
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
            savepath = savepath + ".txt"; // 文件名加上后缀
            QFile file;
            file.setFileName(savepath);
            bool isok = file.open(QIODevice::WriteOnly);
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
        }
    }

}
