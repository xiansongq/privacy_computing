#ifndef PAGE8_H
#define PAGE8_H

#include <QMainWindow>

namespace Ui {
class page8;
}

class page8 : public QMainWindow
{
    Q_OBJECT

public:
    explicit page8(QWidget *parent = nullptr);
    ~page8();

private:
    Ui::page8 *ui;
};

#endif // PAGE8_H
