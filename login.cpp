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

void LogIn::on_loginButton_clicked()
{
    //登入

    MyBoss boss;
    LogIn login;
    MyStaff staff;

    //先检查是否是老板
    if(this->ui->name->text().trimmed() == tr("boss") &&
            this->ui->passward->text().trimmed()== tr("123"))  //去除lineEdit内的用户名和密码进行校验
    {
        //登陆成功后显示对话框
        close();
        boss.exec();

    }
    //然后检查是否是员工
    else if(this->ui->name->text().trimmed() == tr("") &&
            this->ui->passward->text().trimmed()== tr(""))  //去除lineEdit内的用户名和密码进行校验
    {
        //登陆成功后显示对话框
        close();
        staff.exec();
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

        //        query.exec(QString("select *from CUSTOMERS where customer_id = %1").arg(CustomerIdLogin));

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
                //fixme: the init have run twice
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

    //    if(this->ui->name->text().trimmed() == tr("client") &&
    //            this->ui->passward->text().trimmed()== tr("123"))  //去除lineEdit内的用户名和密码进行校验
    //    {
    //        //登陆成功后显示对话框
    //        close();
    //        client.exec();
    //    }

}

void LogIn::on_logoutButton_clicked()
{
    //登出
    close();
}


void LogIn::on_register_2_clicked()
{
    MyRegister r;
    r.exec();
}






//测试事务rollback
//void LogIn::on_pushButton_clicked()
//{
//    QSqlQuery query;
//    query.exec("begin");

//    query.exec("insert into orders (total) values(1)");

//    query.exec("select * from orders");
//    if(query.next())
//    {
//        cout<<query.value("total").toString();
//    }
//    else
//    {
//        cout<<"insert failed";
//    }

//    query.exec("rollback");
//    query.exec("select * from orders");
//    if(query.next())
//    {
//        cout<<"rollback failed";
//    }
//    else
//    {
//        cout<<"rollback succeed";
//    }
//}


//测试加锁 刚开始的时候，表不可为空，否则加不上锁
//void LogIn::on_pushButton_clicked()
//{
//    cout<<"click";
//    QSqlQuery query;
//    query.exec("begin");
//    cout<<"begin";
//    //query.exec("select * from orders for update nowait");

//    int goodsId = 220;
//    int amount =2;//购买量

//    //判断库存
//    query.prepare("select amount from GOODS where goods_id=:goods_id for update nowait");
//    query.bindValue(":goods_id",goodsId);
//    query.exec();
//    cout<<"begin to select";

//    //类似于忙等待
//    while (query.lastError().databaseText() != "")
//    {
//        query.prepare("select amount from GOODS where goods_id=:goods_id for update nowait");
//        query.bindValue(":goods_id",goodsId);
//        query.exec();
//        cout<<"trying to lock table GOODS";
//        Sleep(2000);
//    }

//    if (query.lastError().databaseText() == "")
//    {
//        cout<<"lock succeed!";
//        //query.exec("insert into orders (total) values(1)");
//       // query.exec("select * from orders");

//        if(query.next())
//        {
//            if (query.value("amount").toInt() >= amount)
//            {
//                // 库存足够 deal with the tables 减库存
//                cout<<"enough";
//                query.exec("commit");
//                cout<<"commit";
////                query.prepare("update GOODS set amount=amount-:amount where goods_id=:goods_id");
////                query.bindValue(":amount",amount);
////                query.bindValue(":goods_id",goodsId);
////                query.exec();
////                cout<<"in updating goods:"<<query.lastError();

////                //删购物车内的
////                query.prepare("delete from SHOPPING_CHARTS where goods_id=:goods_id and customer_id=:customer_id");
////                query.bindValue(":goods_id",goodsId);
////                query.bindValue(":customer_id",customerId);
////                query.exec();
////                cout<<"in deleting from SHOPPING_CHARTS"<<query.lastError();

//            }
//            else //not enough to sell
//            {
//                //todo: alert that not enough to sell 一个商品库存不足就弹一个窗吗？？
//               // QMessageBox::warning(this,tr("error!!!"),tr("lack of stock!"),QMessageBox::Yes);
//                //pay = false;
//                //break;
//                cout<<"not enough";
//                query.exec("rollback");
//                cout<<"rollback";
//            }

//        }
//        else
//        {
//            cout<<"error";
//        }

//    }
//    else
//    {
//        cout<<"lock still fail"<<query.lastError().databaseText();
//        //一般不会
//    }

//}
