#ifndef LOGIN_H
#define LOGIN_H

#include<includes.h>

namespace Ui {
class LogIn;
}

class LogIn : public QDialog
{
    Q_OBJECT

public:
    explicit LogIn(QWidget *parent = nullptr);
    ~LogIn();

private slots:
    void on_loginButton_clicked();

    void on_logoutButton_clicked();

    void on_register_2_clicked();

    //void on_pushButton_clicked();

    //void on_pushButton_clicked();

private:
    Ui::LogIn *ui;
};

#endif // LOGIN_H
