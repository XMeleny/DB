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
        QMessageBox::warning(this,tr("success"),tr("welcome!"),QMessageBox::Yes);
        this->close();
    }
    else//检查发现该账号已经被注册
    {
        QMessageBox::warning(this,tr("fail"),tr("account is already existed"),QMessageBox::Yes);
    }
}


//检查用户输入的手机号是否已经被注册过
bool MyRegister::registerCheck(QString customerId)
{
    QSqlQuery query;
    query.prepare("select * from CUSTOMERS where customer_id=:customer_id");
    query.bindValue(":customer_id",customerId);
    query.exec();
    cout<<query.lastQuery();

    if (query.next())
        return false;//该手机号已被注册

    return true;//该手机号未被注册
}
