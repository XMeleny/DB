#include "myclient.h"
#include "ui_myclient.h"
#include"login.h"
#include"connection.h"
MyClient::MyClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyClient)
{
    ui->setupUi(this);
    cout<<customerId;

    goods_model=new QSqlTableModel(this);

    shoppingWidget=new QWidget;
    shoppingLayout=new QVBoxLayout;
    shoppingWidget->setLayout(shoppingLayout);
    ui->verticalLayout_3->addWidget(shoppingWidget);

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
    cout<<"init client"<<endl;

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


    //购物车初始化:
    updateShoppingCharts();
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
    ui->id->setText(record.value("goods_id").toString());
}


//todo:加入购物车
void MyClient::on_addShoppingCart_clicked()
{
    int tempGoodsId=ui->id->text().toInt();
    QString tempGoodsName=ui->name->text();
    int tempAmount=ui->spinBox->value();
    float tempPrice=ui->price->text().toFloat();

    if(tempAmount==0)//todo:判断加购数量要小于库存，窗口提醒
    {
        return;
    }


    //判断主键是否存在（即该用户是否曾经将该物品加购）
    QSqlQuery query;
    query.prepare("select * from shopping_charts where goods_id=:goods_id and customer_id=:customer_id");
    query.bindValue(":goods_id",tempGoodsId);
    query.bindValue(":customer_id",customerId);
    query.exec();
    cout<<query.lastError();

    //todo:加购时减少库存
    //判断主键是否已经存在，如果存在，则加数量即可
    if (query.next())
    {
        //更新数据库
        query.prepare("update shopping_charts set amount=amount+?, money=money+? "
                      "where goods_id=? and customer_id=?");
        query.addBindValue(tempAmount);
        query.addBindValue(tempPrice);
        query.addBindValue(tempGoodsId);
        query.addBindValue(customerId);
        query.exec();
        cout<<query.lastError();

        //更新购物车
        updateShoppingCharts();
    }
    else//如果不在，新添数量
    {
        //更新数据库
        query.prepare("insert into shopping_charts (goods_id,customer_id,money,amount) "
                      "values (?,?,?,?)");
        query.addBindValue(tempGoodsId);
        query.addBindValue(customerId);
        query.addBindValue(ui->sum->text());
        query.addBindValue(tempAmount);
        query.exec();
        cout<<query.lastError();

        //更新购物车
        updateShoppingCharts();
    }

}


//todo: 下单，提取勾选的货物，扣钱，删除已购买货物（小心数量）
void MyClient::on_Btn_buy_clicked()
{
    cout<<"click the 下单 button"<<endl;

}

void MyClient::updateShoppingCharts()
{
    cout<<"in update shoppingCharts";

    //warning:it would find all widgets includes checkbox and
    QList<QWidget*> widgetList=shoppingWidget->findChildren<QWidget*>();

    //delete all
    for(int i=0;i<widgetList.length();i++)
    {
        QWidget *widget=widgetList.at(i);
        shoppingLayout->removeWidget(widget);
        widget->deleteLater();
    }

    //add all
    QSqlQuery query;
    query.prepare("select * from shopping_charts where customer_id=:customer_id");
    query.bindValue(":customer_id",customerId);
    query.exec();

    while(query.next())
    {
        //获得goods_id
        int tempGoodsId=query.value("goods_id").toInt();
        //goods_name
        QSqlQuery temp;
        temp.prepare("select goods_name from GOODS where goods_id=:goods_id");
        temp.bindValue(":goods_id",tempGoodsId);
        temp.exec();
        QString tempGoodsName;
        if (temp.next())
        {
            tempGoodsName=temp.value("goods_name").toString();
        }

        //new the object and set there text
        QWidget *record=new QWidget;
        QHBoxLayout *hLayout=new QHBoxLayout;
        QCheckBox *buy=new QCheckBox("");
        QLabel *goodsName=new QLabel(tempGoodsName);
        QLabel *amount= new QLabel(query.value("amount").toString());
        QLabel *sumMoney=new QLabel(QString::number((query.value("amount").toInt()*query.value("money").toDouble())));

        //set the object's name, making it possible to select and buy

//        hLayout->setObjectName("hLayout"+QString::number(tempGoodsId));
//        buy->setObjectName("buy"+QString::number(tempGoodsId));
//        goodsName->setObjectName("goodsName"+QString::number(tempGoodsId));
//        amount->setObjectName("amount"+QString::number(tempGoodsId));
//        sumMoney->setObjectName("sumMoney"+QString::number(tempGoodsId));

        //dynamically add widgets
        hLayout->addWidget(buy);
        hLayout->addWidget(goodsName);
        hLayout->addWidget(amount);
        hLayout->addWidget(sumMoney);
        record->setLayout(hLayout);
        shoppingLayout->addWidget(record);
    }
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

void MyClient::on_deleteButton_clicked()
{
    cout<<"on delete button click";
    updateShoppingCharts();
}
