#include "myclient.h"
#include "ui_myclient.h"
#include"login.h"
#include"connection.h"
MyClient::MyClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyClient)
{
    //todo: 获得登录的客户id，才能生成购物车界面
    ui->setupUi(this);
    //cout<<customerId;
    goods_model=new QSqlTableModel(this);
    initMyClient();
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


    //todo:初始化购物车,didn't test yet
    query.prepare("select * from shopping_charts where customer_id=:customer_id");
    query.bindValue(":customer_id",customerId);
    query.exec();

    //todo:change the id into name
    QHBoxLayout *hLayout=new QHBoxLayout;
    QLabel *buy=new QLabel("buy");
    QLabel *goodsName=new QLabel("goods_id");
    QLabel *amount= new QLabel("amount");
    QLabel *sumMoney=new QLabel("sumMoney");
    hLayout->addWidget(buy);
    hLayout->addWidget(goodsName);
    hLayout->addWidget(amount);
    hLayout->addWidget(sumMoney);
    ui->verticalLayout_shoppingChart->addLayout(hLayout);

    while(query.next())
    {
        cout<<"record in shopping chart"<<endl;
        QHBoxLayout *hLayout=new QHBoxLayout;
        QCheckBox *buy=new QCheckBox("");
        QLabel *goodsName=new QLabel(query.value("goods_id").toString());
        QLabel *amount= new QLabel(query.value("amount").toString());
        QLabel *sumMoney=new QLabel(query.value("money").toString());

        hLayout->addWidget(buy);
        hLayout->addWidget(goodsName);
        hLayout->addWidget(amount);
        hLayout->addWidget(sumMoney);

        ui->verticalLayout_shoppingChart->addLayout(hLayout);
    }

}

//点击右侧商品，左边修改
void MyClient::on_tableView_clicked(const QModelIndex &index)
{

    cout<<"in on_tableView_click function"<<endl;
    //    onTableSelectChange(1);
    goods_model->setTable("GOODS");
    goods_model->select();

    QSqlRecord record=goods_model->record(index.row());

    ui->name->setText(record.value("goods_name").toString());
    ui->price->setText(record.value("price").toString());
    ui->spinBox->setValue(0);
    ui->comboBox->setCurrentText(record.value("kind").toString());
    ui->stock->setText(record.value("amount").toString());
    ui->sum->setText("0");
}


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



void MyClient::on_name_currentTextChanged(const QString &arg1)
{
    goods_model->setTable("GOODS");
    goods_model->setFilter(QObject::tr("kind= '%1'").arg(arg1));
    goods_model->select();
    ui->tableView->setModel(goods_model);
}

void MyClient::on_pushButton_clicked()
{
    goods_model->setTable("GOODS");
    goods_model->select();
    ui->tableView->setModel(goods_model);
}


void MyClient::on_spinBox_valueChanged(int arg1)
{
    float price1=ui->price->text().toFloat();
    float sum=arg1*price1;
    QString sums = QString("%1").arg(sum);
    ui->sum->setText(sums);
}
