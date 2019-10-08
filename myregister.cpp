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
    QString name,id,psw;
    name=this->ui->name->text().trimmed();
    id=this->ui->id->text().trimmed();
    psw=this->ui->psw->text().trimmed();


}
