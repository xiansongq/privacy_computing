#ifndef PAGE9_H
#define PAGE9_H

#include <QMainWindow>

namespace Ui {
class page9;
}

class page9 : public QMainWindow
{
    Q_OBJECT

public:
    explicit page9(QWidget *parent = nullptr);
    ~page9();

private:
    Ui::page9 *ui;
};

#endif // PAGE9_H
