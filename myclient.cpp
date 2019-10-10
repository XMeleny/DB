#include "myclient.h"
#include "ui_myclient.h"
#include"login.h"
#include"connection.h"
MyClient::MyClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyClient)
{

    ui->setupUi(this);
    //cout<<customerId;
    //initMyClient();
}

MyClient::~MyClient()
{
    delete ui;
}

void MyClient::on_loginAgain_clicked()
{
    //重新登录
    close();
    LogIn login;
    login.exec();
}

//界面初始化
void MyClient::initMyClient()
{
    QStringList strings;
    ui->toolBox->setCurrentIndex(0);
    QSqlQuery query;

    //将商品类型显示到comboBox，测试通过
    query.exec("select kind from GOODS;");
    while(query.next())
    {
        strings.append(query.value(0).toString());
        //cout<<query.value(0);
    }
    QCompleter* com=new QCompleter(strings,this);
    ui->comboBox->clear();
    ui->comboBox->addItems(strings);
    ui->comboBox->setCompleter(com);

    //将商品详情显示到tableView，测试通过
    QSqlTableModel *goods_model=new QSqlTableModel(this);
    goods_model->setTable("GOODS");
    goods_model->select();
    ui->tableView->setModel(goods_model);

    //显示顾客当前余额
    query.prepare("select money from CUSTOMERS where customer_id = ?");
    query.addBindValue(customerId);
    query.exec();
    query.next();
    //float remaining = query.value(0).toFloat();
    QString remaining = QString("%1").arg(query.value(0).toFloat());
    ui->remaining->setText(remaining);
    //cout<<remaining;


    //初始化购物车
    query.prepare("select * from shoppingChart where id=:id");
//    query.bindValue(":id",id);
//    query.exec();

    while(query.next())
    {

    }

}

//点击右侧商品，左边修改
//void MyClient::on_tableView_clicked(const QModelIndex &index)
//{
//    onTableSelectChange(1);
//}

//void MyClient::onTableSelectChange(int row)
//{
//    int r=1;
//    if(row!=0)
//        r=ui->tableView->currentIndex().row();
//    QModelIndex index;
//    QSqlTableModel *goods_model=new QSqlTableModel(this);
//    goods_model->setTable("GOODS");
//    goods_model->select();
//    index=goods_model->index(r,4);//售价
//    ui->price->setText(goods_model->data(index).toString());
//    index=goods_model->index(r,2);//库存
//    ui->stock->setText(goods_model->data(index).toString());
//    index=goods_model->index(r,4);//总价
//    ui->price->setText(goods_model->data(index).toString());
//    QSqlQuery query; //类别
//    query.exec(QString("select kind from GOODS where goods_name='%1'").arg(ui->name->text()));
//    query.next();
//    ui->comboBox->setCurrentText(query.value(0).toString());

//}

//加入购物车--添加复选框
void MyClient::on_addShoppingCart_clicked()
{

}

//下单--清空复选框--扣钱
void MyClient::on_check_clicked()
{

}

//充值
void MyClient::on_charge_clicked()
{
    float rechargeMoney = ui->ChargeMoney->text().toFloat();

    QSqlQuery query;
    query.prepare("select * from CUSTOMERS where customer_id = ?");
    query.addBindValue(customerId);
    query.exec();

    if (query.next())
    {
        float currentMoney = query.value(4).toFloat();
        query.prepare("update CUSTOMERS set money = ? where customer_id = ?");
        query.addBindValue(rechargeMoney+currentMoney);
        query.addBindValue(customerId);
        query.exec();
        //TODO:弹窗显示充值成功？
        cout<<"充值成功!";

        //充值完 更新当前余额
        query.prepare("select money from CUSTOMERS where customer_id = ?");
        query.addBindValue(customerId);
        query.exec();
        query.next();
        //float remaining = query.value(0).toFloat();
        QString remaining = QString("%1").arg(query.value(0).toFloat());
        ui->remaining->setText(remaining);
    }
    else//一般不会出现的情况
        cout<<"出错";
}


