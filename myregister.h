#ifndef MYREGISTER_H
#define MYREGISTER_H

#include<includes.h>

namespace Ui {
class MyRegister;
}

class MyRegister : public QDialog
{
    Q_OBJECT

public:
    explicit MyRegister(QWidget *parent = nullptr);
    ~MyRegister();

private slots:

    void on_close_clicked();

    void on_register_2_clicked();

    bool registerCheck(QString);

private:
    Ui::MyRegister *ui;
};

#endif // MYREGISTER_H
