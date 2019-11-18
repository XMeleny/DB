#include "login.h"
#include "myclient.h"
#include "myboss.h"
#include "mystaff.h"
#include "ui_login.h"
#include "myregister.h"
#include"connection.h"
#include"includes.h"


LogIn::LogIn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogIn)
{
    this->setWindowTitle(tr("login"));
    ui->setupUi(this);
    ui->passward->setEchoMode(QLineEdit::Password);//当输入密码时，显示为*******
}

LogIn::~LogIn()
{
    delete ui;
}

/*
 * function:login
 * input：user_name, password
 * output:login result and open the responding window
*/
void LogIn::on_loginButton_clicked()
{
    MyBoss boss;
    LogIn login;
    MyStaff staff;

    //先检查是否是老板
    if(this->ui->name->text().trimmed() == tr("boss"))  //去除lineEdit内的用户名和密码进行校验
    {
        if (this->ui->passward->text().trimmed()== tr("123"))//password right
        {
            //登陆成功后显示对话框
            close();
            boss.exec();
        }
        else
        {
            QMessageBox::warning(this,tr("error!!!"),tr("wrong password"),QMessageBox::Yes);
        }
    }
    //然后检查是否是员工
    else if(this->ui->name->text().trimmed() == tr("")) //去除lineEdit内的用户名和密码进行校验
    {
        if(this->ui->passward->text().trimmed()== tr(""))
        {
            //登陆成功后显示对话框
            close();
            staff.exec();
        }
        else
        {
            QMessageBox::warning(this,tr("error!!!"),tr("wrong password"),QMessageBox::Yes);
        }
    }
    //然后检查是否是顾客、或者账号不存在
    else
    {
        QSqlQuery query;

        //检查账号是否存在
        QString CustomerIdLogin = ui->name->text();
        query.prepare("select *from CUSTOMERS where customer_id = :id");
        query.bindValue(":id",CustomerIdLogin);
        query.exec();

        if (query.next() == false)
        {

            //账号不存在，弹窗提示用户先注册
            cout<<"账号不存在，请先注册~";

            QMessageBox::warning(this,tr("error!!!"),tr("please register first!"),QMessageBox::Yes);
            ui->name->clear();  //清空姓名nameLine
            ui->passward->clear();  //清空密码passwardLine
            ui->name->setFocus();  //将鼠标重新定位到nameLine
        }
        else
        {
            //账号存在，检查密码是否正确
            if (query.value("psw").toString() == ui->passward->text())
            {
                //密码正确，登录成功，跳转到购物界面
                cout<<"login success";
                //登陆成功后显示对话框
                close();
                MyClient client;
                client.customerId = CustomerIdLogin;
                client.initMyClient();
                client.exec();

            }
            else
            {
                //密码错误，登录失败
                cout<<"密码错误,登录失败";
                QMessageBox::warning(this,tr("error!!!"),tr("wrong account or password!"),QMessageBox::Yes);
                ui->name->clear();  //清空姓名nameLine
                ui->passward->clear();  //清空密码passwardLine
                ui->name->setFocus();  //将鼠标重新定位到nameLine
            }
        }
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

