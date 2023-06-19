#ifndef PAGE10_H
#define PAGE10_H

#include <QMainWindow>

namespace Ui {
class page10;
}

class page10 : public QMainWindow
{
    Q_OBJECT

public:
    explicit page10(QWidget *parent = nullptr);
    ~page10();

private:
    Ui::page10 *ui;
};

#endif // PAGE10_H
