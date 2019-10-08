#include "myregister.h"
#include "ui_myregister.h"

MyRegister::MyRegister(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyRegister)
{

    ui->setupUi(this);
}

MyRegister::~MyRegister()
{
    delete ui;
}

void MyRegister::on_close_clicked()
{
    close();
}

void MyRegister::on_register_2_clicked()
{
    //将用户注册信息（姓名 账号 密码）插入数据库
    QSqlQuery query;
    QString customerIdRegister = ui->id->text();

    if (registerCheck(customerIdRegister) == true)//检查发现该手机号未被注册
    {
        query.prepare("insert into CUSTOMERS(customer_id,customer_name,psw,money) "
                      "values(?,?,?,?)");

        query.addBindValue(ui->id->text());
        query.addBindValue(ui->name->text());
        query.addBindValue(ui->psw->text());
        query.addBindValue(0);
        query.exec();
        cout << "注册成功";

    }
    else//检查发现该手机号已经被注册
    {
        //TODO:弹窗提示该号码已经注册过？
        cout << "注册失败，账号已注册";
    }

}

//检查用户输入的手机号是否已经被注册过
bool MyRegister::registerCheck(QString customerId)
{

    QSqlQuery query;

    query.exec(QString("select *from CUSTOMERS where customer_id = %1").arg(customerId));

    while (query.next())
        return false;//该手机号已被注册

    return true;//该手机号未被注册

}
