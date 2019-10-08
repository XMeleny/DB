#include "myboss.h"
#include "ui_myboss.h"
#include"login.h"
#include"connection.h"


MyBoss::MyBoss(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyBoss)
{
    ui->setupUi(this);
}

MyBoss::~MyBoss()
{
    delete ui;
}

void MyBoss::on_loginAgain_clicked()
{
    close();
LogIn login;
login.exec();

}

void MyBoss::on_pushButton_clicked()
{
    //if(createConnection())
    //{

        QSqlQuery query;
        query.exec("select *from GOODS;");
        while(query.next())
        {
            qDebug()<<query.value(0).toInt()<<query.value(1).toString();
        }
    //}
}
//周结账单
void MyBoss::on_weeklyBill_clicked()
{

}
//月结账单
void MyBoss::on_monthlyBill_clicked()
{

}
//年结账单
void MyBoss::on_yearBill_clicked()
{

}
//添加优惠
void MyBoss::on_addDiscount_clicked()
{

}
//删除优惠
void MyBoss::on_deleteDiscount_clicked()
{

}
