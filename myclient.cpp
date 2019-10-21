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

    goods_model=new QSqlTableModel(this);

    shoppingWidget=new QWidget;
    shoppingLayout=new QVBoxLayout;
    shoppingWidget->setLayout(shoppingLayout);
    ui->verticalLayout_3->addWidget(shoppingWidget);

    //initMyClient();
}

MyClient::~MyClient()
{
    delete ui;
}
//重新登录
void MyClient::on_loginAgain_clicked()
{

    close();
    LogIn login;
    login.exec();
}

//界面初始化
void MyClient::initMyClient()
{
    cout<<"init client"<<endl;
    cout<<customerId;

    ui->spinBox->setValue(0);

    QStringList strings;
    ui->toolBox->setCurrentIndex(0);
    QSqlQuery query;
    strings.append("全部");
    //将商品类型显示到comboBox，且不重复，测试通过
    query.exec("select distinct kind from GOODS;");
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
    //QSqlTableModel *goods_model=new QSqlTableModel(this);
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


//todo：减余额，下单判断库存
void MyClient::on_addShoppingCart_clicked()
{
    int tempGoodsId=ui->id->text().toInt();
    QString tempGoodsName=ui->name->text();
    int tempAmount=ui->spinBox->value();
    float tempPrice=ui->price->text().toFloat();


    //判断加购数量，如果为0或大于库存，提示错误，不进行更新

    int goodsAmount = showGoodsAmount(tempGoodsId);
    if (tempAmount > goodsAmount || tempAmount == 0)
    {
        //弹窗报错
        QMessageBox::warning(this,tr("error!!!"),tr("illegal amounts!"),QMessageBox::Yes);
        return;
    }

    //

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

    //判断主键是否已经存在，如果存在，则加数量即可
    if (query.next())//存在，update
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
    else//不在，insert
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


int MyClient::showGoodsAmount(int goodsId)//根据id查找商品库存
{
    QSqlQuery query;

    query.prepare("select *from GOODS where goods_id = ?");
    query.addBindValue(goodsId);
    query.exec();

    if(query.next())
    {
        return query.value(2).toInt();
    }
    else
    {
        cout<<"出错";
        return 0;
    }
}






//todo: 下单，提取勾选的货物，扣钱，删除已购买货物（小心数量）
void MyClient::on_Btn_buy_clicked()
{

    cout<<"click the 下单 button"<<endl;
    QSqlQuery query;
    QList<QCheckBox*> checkboxList=shoppingWidget->findChildren<QCheckBox*>();
    for (int i=0;i<checkboxList.length();i++)
    {
        QCheckBox *checkbox=checkboxList.at(i);
        if (checkbox->checkState())//被选中，此时objecName为buy+goodsId
        {

    ui->toolBox->setCurrentIndex(3);
    }
        }

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
        //get goods_id
        int tempGoodsId=query.value("goods_id").toInt();
        //get goods_name
        QSqlQuery temp;
        temp.prepare("select goods_name from GOODS where goods_id=:goods_id");
        temp.bindValue(":goods_id",tempGoodsId);
        temp.exec();

        QSqlQuery query1;
        query1.prepare("select * from goods where goods_id=:goods_id");
        query1.bindValue(":goods_id",tempGoodsId);
        query1.exec();

        QString tempGoodsName;
        if (temp.next())
        {
            tempGoodsName=temp.value("goods_name").toString();
        }

        float tempGoodsSum;
        if (query1.next())
        {
            float price=query1.value("price").toFloat();
            tempGoodsSum=price*query.value("amount").toFloat();
        }
        QString sum=QString("%1").arg(tempGoodsSum);

        //new the object and set there text
        QWidget *record=new QWidget;
        QHBoxLayout *hLayout=new QHBoxLayout;
        QCheckBox *buy=new QCheckBox("");
        QLabel *goodsName=new QLabel(tempGoodsName);
        QLabel *amount= new QLabel(query.value("amount").toString());
        QLabel *sumMoney=new QLabel(sum);

        //set the object's name, making it possible to select and buy

        record->setObjectName("record"+QString::number(tempGoodsId));
        buy->setObjectName("buy"+QString::number(tempGoodsId));
        goodsName->setObjectName("goodsName"+QString::number(tempGoodsId));
        amount->setObjectName("amount"+QString::number(tempGoodsId));
        sumMoney->setObjectName("sumMoney"+QString::number(tempGoodsId));

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
        float currentMoney = query.value(3).toFloat();
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



//显示所有商品

//void MyClient::on_name_currentTextChanged(const QString &arg1)
//{
//    goods_model->setTable("GOODS");
//    goods_model->setFilter(QObject::tr("kind= '%1'").arg(arg1));
//    goods_model->select();
//    ui->tableView->setModel(goods_model);
//}


void MyClient::on_pushButton_clicked()
{
    ui->comboBox->setCurrentText("全部");
    goods_model->setTable("GOODS");
    goods_model->select();
    ui->tableView->setModel(goods_model);
    ui->id->setText("");
    ui->name->setText("");
    ui->stock->setText("");
    ui->price->setText("");
    ui->sum->setText("");
    ui->spinBox->setValue(0);
    ui->sum->setText("");


}

//选择数量

void MyClient::on_spinBox_valueChanged(int arg1)
{
    float price1=ui->price->text().toFloat();
    float sum=arg1*price1;
    QString sums = QString("%1").arg(sum);
    ui->sum->setText(sums);
}



//点击右侧商品，左边修改
void MyClient::on_tableView_clicked(const QModelIndex &index)
{


    cout<<"in on_tableView_click())  index:"<<index<<endl;
    //    onTableSelectChange(1);
//    goods_model->setTable("GOODS");
//    goods_model->select();
//    QSqlRecord record=goods_model->record(index.row());
//    ui->name->setText(record.value("goods_name").toString());
//    ui->price->setText(record.value("price").toString());
//    ui->spinBox->setValue(1);
//    ui->comboBox->setCurrentText(record.value("kind").toString());
//    ui->stock->setText(record.value("amount").toString());
//    ui->sum->setText("0");
//    ui->id->setText(record.value("goods_id").toString());
//

    int r;
    r=ui->tableView->currentIndex().row();
    //QModelIndex index;

    QModelIndex currentIndex;
    goods_model->setTable("GOODS");
    if(ui->comboBox->currentText()=="全部")
    {

    }
    else
    {
        goods_model->setFilter(QObject::tr("kind= '%1'").arg(ui->comboBox->currentText()));
    }
    goods_model->select();//获取表
    currentIndex=goods_model->index(r,0);//id
    ui->id->setText(goods_model->data(currentIndex).toString());

    currentIndex=goods_model->index(r,1);//名称
    ui->name->setText(goods_model->data(currentIndex).toString());

    currentIndex=goods_model->index(r,2);//库存
    ui->stock->setText(goods_model->data(currentIndex).toString());

    currentIndex=goods_model->index(r,4);//单价
    ui->price->setText(goods_model->data(currentIndex).toString());

    QSqlQuery query; //类别
    query.exec(QString("select kind from GOODS where goods_id='%1'").arg(ui->id->text()));
    query.next();
    ui->comboBox->setCurrentText(query.value(0).toString());
//    QSqlQuery query; //类别
//    query.exec(QString("select kind from discounts where discount_id='%1'").arg(ui->id->text()));
//    query.next();
    //ui->kind->setCurrentText(query.value(0).toString());

    goods_model->setTable("GOODS");
    goods_model->setFilter(QObject::tr("kind= '%1'").arg(ui->comboBox->currentText()));
    goods_model->select();
    ui->tableView->setModel(goods_model);

    ui->spinBox->setValue(1);
    on_spinBox_valueChanged(1);
}

//根据左边种类筛选右侧商品
void MyClient::on_comboBox_currentTextChanged(const QString &arg1)
{
    cout<<"in on_comboBox_currentTextChanged()";
    goods_model->setTable("GOODS");
    if (ui->comboBox->currentText()=="全部")
    {
        on_pushButton_clicked();
        return;
    }

    goods_model->setFilter(QObject::tr("kind= '%1'").arg(arg1));
    goods_model->select();
    ui->tableView->setModel(goods_model);

}

//TODO:搜索功能
////按名字筛选？？？
//void MyClient::on_name_currentTextChanged(const QString &arg1)
//{
//    goods_model->setTable("GOODS");
//    goods_model->setFilter(QObject::tr("goods_name= '%1'").arg(arg1));
//    goods_model->select();
//    ui->tableView->setModel(goods_model);
//}


//确定下单按钮
void MyClient::on_pushButton_2_clicked()
{
    QSqlQuery query;
    float sum=0;
    QString goodsIds,goodsAmounts,id,amt;
    QList<QCheckBox*> checkboxList=shoppingWidget->findChildren<QCheckBox*>();
    for (int i=0;i<checkboxList.length();i++)
    {
        QCheckBox *checkbox=checkboxList.at(i);
        if (checkbox->checkState())//被选中，此时objecName为buy+goodsId
        {

            //获得goodsId
            int goodsId=checkbox->objectName().mid(3).toInt();
            id=QString::number(goodsId);
            //订单中所以商品的id
            goodsIds=goodsIds+" "+id;
            int amount=shoppingWidget->findChild<QLabel*>("amount"+QString::number(goodsId))->text().toInt();
            amt=QString::number(amount);
            //订单中所以商品id对应的数量
            goodsAmounts=goodsAmounts+" "+amt;

            float sumMoney=shoppingWidget->findChild<QLabel*>("sumMoney"+QString::number(goodsId))->text().toFloat();
            sum+=sumMoney;
//            cout<<goodsId;
            //已完成：根据购物车勾选状态获取要购买的商品的goods_id
            //todo：处理数据库
            //todo：判断库存。库存足够，删购物车记录，插入订单记录，修改顾客钱钱，修改商品库存；
            //todo:库存不够，则跳出弹窗，不够库存
            //todo:check goods's amount
            query.prepare("select amount from GOODS where goods_id=:goods_id");
            query.bindValue(":goods_id",goodsId);
            query.exec();
//            cout<<query.lastError();
            if (query.next())
            {
                if (query.value("amount")>amount)//enougn to sell
                {
                    //todo:deal with the tables
                    query.prepare("update GOODS set amount=amount-:amount where goods_id=:goods_id");
                    query.bindValue(":amount",amount);
                    query.bindValue(":goods_id",goodsId);
                    query.exec();
                    cout<<"in updating goods:"<<query.lastError();

                    query.prepare("delete from SHOPPING_CHARTS where goods_id=:goods_id and customer_id=:customer_id");
                    query.bindValue(":goods_id",goodsId);
                    query.bindValue(":customer_id",customerId);
                    query.exec();
                    cout<<"in deleting from SHOPPING_CHARTS"<<query.lastError();


                }
                else//not enough to sell
                {
                    //todo: alert that not enough to sell
                }
            }
            else//这情况不应该出现
            {
                cout<<"cant find such goods";
            }
        }
    }
    //处理ui
    updateShoppingCharts();
    //todo:insert into orders
    query.prepare("insert into orders(goods_id_list,customer_id,total,timing,address,goods_amount_list,activity_id) values(:goods_id_list,:customer_id,:total,:timing,:address,:goods_amount_list,:activity_id)");
    cout<<goodsIds<<" "<<customerId<<" "<<sum<<" "<<ui->address->text()<<" "<<goodsAmounts;
    query.bindValue(":goods_id_list",goodsIds);
    query.bindValue(":customer_id",customerId);
    query.bindValue(":total",sum);
    QDateTime curDateTime=QDateTime::currentDateTime();
    query.bindValue(":timing",curDateTime);
    query.bindValue(":address",ui->address->text());
    query.bindValue(":goods_amount_list",goodsAmounts);
    //todo:判断可用哪些优惠
//    query.bindValue(":activity_id",);
    query.exec();
    cout<<query.lastError();
}
