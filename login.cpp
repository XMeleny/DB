#include "login.h"
#include "myclient.h"
#include "myboss.h"
#include "mystaff.h"
#include "ui_login.h"
#include "myregister.h"


LogIn::LogIn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogIn)
{
    this->setWindowTitle(tr("登录界面"));
    ui->setupUi(this);
    ui->passward->setEchoMode(QLineEdit::Password);//当输入密码时，显示为*******
}

LogIn::~LogIn()
{
    delete ui;
}

void LogIn::on_loginButton_clicked()
{
    MyClient client;
    MyBoss boss;
    LogIn login;
    MyStaff staff;
    //accept();
    //将用户登录的if语句改成查询数据库中密码的操作
    if(this->ui->name->text().trimmed() == tr("client") &&
            this->ui->passward->text().trimmed()== tr("123"))  //去除lineEdit内的用户名和密码进行校验
    {
        //登陆成功后显示对话框
        close();
        client.exec();

    }
    else if(this->ui->name->text().trimmed() == tr("boss") &&
            this->ui->passward->text().trimmed()== tr("123"))  //去除lineEdit内的用户名和密码进行校验
    {
        //登陆成功后显示对话框
        close();
        boss.exec();

    }
    else if(this->ui->name->text().trimmed() == tr("") &&
            this->ui->passward->text().trimmed()== tr(""))  //去除lineEdit内的用户名和密码进行校验
    {
        //登陆成功后显示对话框
        close();
        staff.exec();
    }
    else
    {
        QMessageBox::warning(this,tr("error!!!"),tr("wrong account or password!"),QMessageBox::Yes);
        ui->name->clear();  //清空姓名nameLine
        ui->passward->clear();  //清空密码passwardLine
        ui->name->setFocus();  //将鼠标重新定位到nameLine
    }

}

void LogIn::on_logoutButton_clicked()
{
    close();
}


void LogIn::on_register_2_clicked()
{
    MyRegister r;
    r.exec();
}
