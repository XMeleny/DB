#include "myclient.h"
#include "ui_myclient.h"
#include"login.h"
#include"connection.h"
#include"includes.h"
MyClient::MyClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyClient)
{
    ui->setupUi(this);
    //cout<<customerId;

    maxcount=10;
    count=0;
    goods_model=new QSqlTableModel(this);
    discountId=0;
    shoppingWidget=new QWidget;
    shoppingLayout=new QVBoxLayout;
    shoppingWidget->setLayout(shoppingLayout);
    ui->verticalLayout_8->addWidget(shoppingWidget);

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
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnHidden(3,true);
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

//加入购物车按钮
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

        QMessageBox::warning(this,tr("error!!!"),tr("illegal amounts!"),QMessageBox::Yes);
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
        query.prepare("update shopping_charts set amount=amount+:amount "
                      "where goods_id=:goods_id and customer_id=:customer_id");
        query.bindValue(":amount",tempAmount);
        query.bindValue(":goods_id",tempGoodsId);
        query.bindValue(":customer_id",customerId);
        query.exec();
    }
    else//不在，insert
    {
        //        if(count>maxcount)
        //        {
        //                QMessageBox::warning(this,tr("error!!!"),tr("your shopping chart is full"),QMessageBox::Yes);
        //                return;
        //    }
        query.prepare("select count(goods_id) from shopping_charts "
                      "where customer_id =:customer_id");
        query.bindValue(":customer_id",customerId);
        query.exec();
        cout<<query.lastQuery();
        cout<<query.lastError();
        if (query.next())
        {
            if (query.value(0).toInt()>=maxcount)
            {
                QMessageBox::warning(this,tr("error!!!"),tr("your shopping chart is full"),QMessageBox::Yes);
                return;
            }
        }


        //更新数据库
        query.prepare("insert into shopping_charts (goods_id,customer_id,amount) "
                      "values (?,?,?)");
        query.addBindValue(tempGoodsId);
        query.addBindValue(customerId);
        query.addBindValue(tempAmount);
        query.exec();
        cout<<"in add shopping_chart"<<query.lastError();
    }
    //更新购物车
    updateShoppingCharts();

}

//根据id查找商品库存
int MyClient::showGoodsAmount(int goodsId)
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
        cout<<"error";
        return 0;
    }
}

//提取勾选的货物 下单按钮
void MyClient::on_Btn_buy_clicked()
{
    cout<<"click the order button"<<endl;
    QSqlQuery query;
    float sum=0;
    QString goodsIds,goodsAmounts,id,amt;
    QList<QCheckBox*> checkboxList=shoppingWidget->findChildren<QCheckBox*>();
    QDate curDate=QDate::currentDate();
    for (int i=0;i<checkboxList.length();i++)
    {
        QCheckBox *checkbox=checkboxList.at(i);
        if (checkbox->checkState())//被选中，此时objecName为buy+goodsId
        {
            ui->toolBox->setCurrentIndex(3);

            //获得goodsId
            int goodsId=checkbox->objectName().mid(3).toInt();
            id=QString::number(goodsId);

            //顾客要买的数量
            int amount=shoppingWidget->findChild<QSpinBox*>("amount"+QString::number(goodsId))->text().toInt();
            amt=QString::number(amount);


            float sumMoney=shoppingWidget->findChild<QLabel*>("sumMoney"+QString::number(goodsId))->text().toFloat();
            sum+=sumMoney;
            //            cout<<goodsId;
            //已完成：根据购物车勾选状态获取要购买的商品的goods_id
            //处理数据库？？
            //判断库存。库存足够，删购物车记录，插入订单记录，修改顾客钱钱，修改商品库存；
            //该商品仓库库存
            //            int goodsAmount = showGoodsAmount(goodsId);

            //            if (amount > goodsAmount )
            //            {
            //                //库存不够 弹窗报错
            //                QMessageBox::warning(this,tr("error!!!"),tr("lack of stock!"),QMessageBox::Yes);
            //                return;
            //            }
            //            //else 库存足够 则库存减少
            //            goodsAmount -= amount;


            //处理ui

            //判断可用哪些优惠
            QSqlQuery tempQuery1;
            tempQuery1.prepare("select * from discounts where start_time<:curDate and :curDate<end_time");
            tempQuery1.bindValue(":curDate",curDate);
            tempQuery1.exec();
            float tempSum=sum;
            while(tempQuery1.next())
            {
                if(tempQuery1.value("kind")=="满100减10")
                {
                    if(sum>=100&&tempSum>sum-10)
                    {
                        tempSum=sum-10;
                        discountId=tempQuery1.value("discount_id").toInt();
                        ui->discount_kind->setText(tempQuery1.value("kind").toString());
                    }
                }
                else if(tempQuery1.value("kind")=="满200减20")
                {
                    if(sum>=200&&tempSum>sum-20)
                    {
                        tempSum=sum-20;
                        discountId=tempQuery1.value("discount_id").toInt();
                        ui->discount_kind->setText(tempQuery1.value("kind").toString());
                    }
                }
                else if(tempQuery1.value("kind")=="满300减30")
                {
                    if(sum>=300&&tempSum>sum-30)
                    {
                        tempSum=sum-30;
                        discountId=tempQuery1.value("discount_id").toInt();
                        ui->discount_kind->setText(tempQuery1.value("kind").toString());
                    }
                }
                else if(tempQuery1.value("kind")=="七折")
                {
                    if(tempSum>sum*0.7)
                    {
                        tempSum=sum*0.7;
                        discountId=tempQuery1.value("discount_id").toInt();
                        ui->discount_kind->setText(tempQuery1.value("kind").toString());
                    }
                }
                else if(tempQuery1.value("kind")=="八折")
                {

                    if(tempSum>sum*0.8)
                    {
                        tempSum=sum*0.8;
                        discountId=tempQuery1.value("discount_id").toInt();
                        ui->discount_kind->setText(tempQuery1.value("kind").toString());
                    }
                }
                else if(tempQuery1.value("kind")=="九折")
                {
                    if(tempSum>sum*0.9)
                    {
                        tempSum=sum*0.9;
                        discountId=tempQuery1.value("discount_id").toInt();
                        ui->discount_kind->setText(tempQuery1.value("kind").toString());
                    }

                }
            }
            //    cout<<tempQuery1.lastError();

            //显示原价、折扣价在确认下单页面
            ui->original_cost->setText(QString::number(sum));
            ui->discounted_cost->setText(QString::number(tempSum));
        }
    }
}

//购物车显示
void MyClient::updateShoppingCharts()
{
    cout<<"in update shoppingCharts";

    //warning:it would find all widgets includes checkboxes and labels
    QList<QWidget*> widgetList=shoppingWidget->findChildren<QWidget*>();
    //    QList<QWidget*> widgetList=ui->scrollArea->findChildren<QWidget*>();

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
    //    cout<<query.lastError();

    while(query.next())
    {

        //get goods_id
        int tempGoodsId=query.value("goods_id").toInt();

        QSqlQuery temp;
        temp.prepare("select * from goods where goods_id=:goods_id");
        temp.bindValue(":goods_id",tempGoodsId);
        temp.exec();
        //get goods_name ,price
        QString tempGoodsName;
        float tempGoodsSum;
        int tempGoodsAmount;
        float tempGoodsPrice;
        if (temp.next())
        {
            tempGoodsName=temp.value("goods_name").toString();
            tempGoodsPrice=temp.value("price").toFloat();
            tempGoodsSum=tempGoodsPrice*query.value("amount").toFloat();
            tempGoodsAmount=temp.value("amount").toInt();
        }

        QString sum=QString("%1").arg(tempGoodsSum);
        int tempAmount = query.value("amount").toInt();

        //new the object and set there text
        QWidget *record=new QWidget;
        QHBoxLayout *hLayout=new QHBoxLayout;
        QCheckBox *buy=new QCheckBox("");
        QLabel *goodsName=new QLabel(tempGoodsName);
        //        QLabel *amount= new QLabel(query.value("amount").toString());
        QSpinBox *amount=new QSpinBox();
        amount->setValue(query.value("amount").toInt());
        amount->setMinimum(1);
        amount->setMaximum(tempGoodsAmount);
        QLabel *sumMoney=new QLabel(sum);

        buy->setMinimumWidth(40);
        buy->setMaximumWidth(40);
        goodsName->setMinimumWidth(120);
        goodsName->setMaximumWidth(120);
        amount->setMinimumWidth(90);
        amount->setMaximumWidth(90);
        sumMoney->setMinimumWidth(100);
        sumMoney->setMaximumWidth(100);

        record->setMinimumHeight(30);

        //判断可不可打钩
        //这是商品的库存
        int goodsAmount = showGoodsAmount(tempGoodsId);

        if ( tempAmount > goodsAmount )
        {
            //库存不够 不可打钩
            buy->setEnabled(false);

        }

        //当amount的spinbox发生改变时
        connect(amount, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                [=](int value)
        {
            //temp不能使用外部变量，而要重新定义，但tempGoodsId这些又可以使用
            QSqlQuery temp;
            temp.prepare("update SHOPPING_CHARTS set amount=:amount "
                         "where goods_id=:goods_id and customer_id=:customer_id");
            temp.bindValue(":amount",value);
            temp.bindValue(":goods_id",tempGoodsId);
            temp.bindValue(":customer_id",customerId);
            temp.exec();
            cout<<temp.lastQuery();
            cout<<"in spinbox change: "<<temp.lastError();

            sumMoney->setText(QString::number(value*tempGoodsPrice));
        });

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

//确定下单按钮 //todo: 下单，扣钱，删除已购买货物（小心数量）
void MyClient::on_pushButton_2_clicked()
{
    //地址是否为空
    QString input = ui->address->text();
    if ( input.isEmpty() )
    {        //输入为空，警告处理
        QMessageBox::warning(this,tr("error!!!"),tr("please input address"),QMessageBox::Yes);
        return;
    }
    cout<<"on click 确定下单";
    float sum=0;
    QString goodsIds,goodsAmounts,id,amt;
    QList<QCheckBox*> checkboxList=shoppingWidget->findChildren<QCheckBox*>();
    QDate curDate=QDate::currentDate();

    //先判断够不够钱 不够钱直接return 但是先不扣钱
    QSqlQuery query;
    query.prepare("select money from CUSTOMERS where customer_id = ?");
    query.addBindValue(customerId);
    query.exec();
    if (query.next())
    {
        float customerMoney = query.value("money").toFloat();
        cout<<"customer money is:"<<customerMoney;

        if (customerMoney < ui->discounted_cost->text().toFloat())
        {
            //TODO:顾客余额不足，提示请先充值？自动跳转？显示支付失败
            //cout<<"余额不足，请先充值";
            QMessageBox::warning(this,tr("error!!!"),tr("not sufficient funds!"),QMessageBox::Yes);
            cout<<"lack of money";
            return;
        }
        else
            cout<<"money is enough";
    }
    else
    {
        //bug: 如果未曾充值，money是不是初始化为0？
        //一般情况下不会出现
        cout<<"error";
        return;
    }

    //够钱，继续判断库存 够库存的商品就扣钱扣库存，不够库存的就不扣，todo：然后统一统一弹窗提示
    query.exec("begin");
    cout<<"begin to check every stock";
    //query.exec("select * from orders for update nowait");
    //cout<<"begin to select";
    //cout<<query.lastError();

    //直接扣库存，订单中只要有一个商品的库存不够，那么整个订单都失败 rollback
    bool pay = true;//判断订单是否能成功
    //TODO:后期把10改成一笔订单中的最大商品数
    int * lackGoodsId = new int [10];//存这笔订单中库存不够的商品id
    int cur = 0;//用来记录数组的当前下标

    for (int i=0;i<checkboxList.length();i++)
    {
        cout<<"in process juding stock";
        QCheckBox *checkbox=checkboxList.at(i);
        if (checkbox->checkState())//被选中，此时objecName为buy+goodsId
        {
            //获得goodsId
            int goodsId=checkbox->objectName().mid(3).toInt();
            id=QString::number(goodsId);
            //订单中所有商品的id
            goodsIds=goodsIds+" "+id;
            int amount=shoppingWidget->findChild<QSpinBox*>("amount"+QString::number(goodsId))->text().toInt();
            amt=QString::number(amount);
            //订单中所有商品id对应的数量
            goodsAmounts=goodsAmounts+" "+amt;

            //float sumMoney=shoppingWidget->findChild<QLabel*>("sumMoney"+QString::number(goodsId))->text().toFloat();
            //sum+=sumMoney;
            //cout<<sumMoney;
            //已完成：根据购物车勾选状态获取要购买的商品的goods_id
            //todo：判断库存。库存足够，删购物车记录，插入订单记录，修改顾客钱钱，修改商品库存；

            //判断库存
            query.prepare("select amount from GOODS where goods_id=:goods_id for update nowait");
            query.bindValue(":goods_id",goodsId);
            query.exec();
            //类似于忙等待
            if (query.lastError().databaseText() != "")
            {
                QMessageBox::warning(this,tr("loading..."),tr("please wait"),QMessageBox::Yes);
                query.prepare("select amount from GOODS where goods_id=:goods_id for update nowait");
                query.bindValue(":goods_id",goodsId);
                query.exec();
                cout<<"trying to lock table GOODS";
                Sleep(2000);
            }
            while (query.lastError().databaseText() != "")
            {
                query.prepare("select amount from GOODS where goods_id=:goods_id for update nowait");
                query.bindValue(":goods_id",goodsId);
                query.exec();
                cout<<"trying to lock table GOODS";
                Sleep(2000);
            }

            if (query.next())
            {
                //enougn to sell  库存量 >= 购买量
                if (query.value("amount").toInt() >= amount)
                {
                    //deal with the tables 减库存
                    query.prepare("update GOODS set amount=amount-:amount where goods_id=:goods_id");
                    query.bindValue(":amount",amount);
                    query.bindValue(":goods_id",goodsId);
                    query.exec();
                    cout<<"in updating GOODS amount:"<<query.lastError();

                    //删购物车内的
                    query.prepare("delete from SHOPPING_CHARTS where goods_id=:goods_id and customer_id=:customer_id");
                    query.bindValue(":goods_id",goodsId);
                    query.bindValue(":customer_id",customerId);
                    query.exec();
                    cout<<"in deleting from SHOPPING_CHARTS"<<query.lastError();

                }
                else //not enough to sell
                {
                    //todo: alert that not enough to sell
                    // QMessageBox::warning(this,tr("error!!!"),tr("lack of stock!"),QMessageBox::Yes);
                    pay = false;
                    lackGoodsId[cur] = goodsId;
                    cur++;
                    cout<<"not enough to sell";
                    continue;
                }
            }
            else//这情况不应该出现
            {
                cout<<"cant find such goods";
            }
        }
    }
    cout<<"successfully judge stock";

    if (pay == false)
    {
        //订单生成失败，有商品库存不足 TODO:弹窗警告
        query.exec("rollback");
        cout<<"rollback";

        //TODO:把数组 lackGoodsId 中的库存不足商品id显示出来 用弹窗？
        delete []lackGoodsId;
        return;
    }
    else
    {
        //订单生成成功，全部钱一起扣  commit刚刚的减少库存、删购物车的事务
        query.exec("commit");
        cout<<"commit checking all stock";

        query.prepare("update CUSTOMERS set money=money-:sumMoney where customer_id=:customer_id");
        query.bindValue(":sumMoney",ui->discounted_cost->text().toFloat());
        query.bindValue(":customer_id",customerId);
        query.exec();
        cout<<"in updating money:"<<query.lastError();

    }

    //处理ui
    //购物车更新
    updateShoppingCharts();

    //insert into orders 生成订单
    query.prepare("insert into orders(goods_id_list,customer_id,total,timing,address,goods_amount_list,activity_id) values(:goods_id_list,:customer_id,:total,:timing,:address,:goods_amount_list,:activity_id)");
    cout<<goodsIds<<" "<<customerId<<" "<<sum<<" "<<ui->address->text()<<" "<<goodsAmounts;
    query.bindValue(":goods_id_list",goodsIds);
    query.bindValue(":customer_id",customerId);

    query.bindValue(":timing",curDate);
    query.bindValue(":address",ui->address->text());
    query.bindValue(":goods_amount_list",goodsAmounts);
    query.bindValue(":total",ui->discounted_cost->text());
    query.bindValue(":activity_id",discountId);
    query.exec();
    cout<<query.lastError();
    ui->address->clear();
    ui->original_cost->clear();
    ui->discount_kind->clear();
    ui->discounted_cost->clear();


    //更新充值界面的当前余额
    query.prepare("select money from CUSTOMERS where customer_id = ?");
    query.addBindValue(customerId);
    query.exec();
    query.next();
    //float remaining = query.value(0).toFloat();
    QString remaining = QString("%1").arg(query.value(0).toFloat());
    ui->remaining->setText(remaining);

    delete []lackGoodsId;

    QMessageBox::warning(this,tr("success"),tr("order success!"),QMessageBox::Yes);
}

//充值
void MyClient::on_charge_clicked()
{
    float rechargeMoney = ui->ChargeMoney->text().toFloat();
    float remainingMoney=ui->remaining->text().toFloat();

    //更新顾客数据库
    if (rechargeMoney!=0)
    {
        QSqlQuery query;
        query.prepare("update CUSTOMERS set money=:money where customer_id=:customer_id");
        query.bindValue(":money",rechargeMoney+remainingMoney);
        query.bindValue(":customer_id",customerId);
        query.exec();
        cout<<query.lastQuery();
    }
    else{
        //todo
        QMessageBox::warning(this,tr("error"),tr("无充值金额！"),QMessageBox::Yes);
    }


    //修改ui
    ui->ChargeMoney->clear();
    ui->remaining->setText(QString::number(rechargeMoney+remainingMoney));

    //    QSqlQuery query;
    //    query.prepare("select * from CUSTOMERS where customer_id = ?");
    //    query.addBindValue(customerId);
    //    query.exec();

    //    if (query.next())
    //    {
    //        float currentMoney = query.value(3).toFloat();
    //        query.prepare("update CUSTOMERS set money = ? where customer_id = ?");
    //        query.addBindValue(rechargeMoney+currentMoney);
    //        query.addBindValue(customerId);
    //        query.exec();
    //        //TODO:弹窗显示充值成功？
    //        cout<<"充值成功!";

    //        //充值完 更新当前余额
    //        query.prepare("select money from CUSTOMERS where customer_id = ?");
    //        query.addBindValue(customerId);
    //        query.exec();
    //        query.next();
    //        //float remaining = query.value(0).toFloat();
    //        QString remaining = QString("%1").arg(query.value(0).toFloat());
    //        ui->remaining->setText(remaining);

    //        //TODO:把刚刚输入框内的数字删掉
    //    }
    //    else//一般不会出现的情况
    //        cout<<"出错";
}

////初始化
//void MyClient::on_pushButton_clicked()
//{
//    ui->comboBox->setCurrentText("all");
//    goods_model->setTable("GOODS");
//    goods_model->select();
//    ui->tableView->setModel(goods_model);
//    ui->id->setText("");
//    ui->name->setText("");
//    ui->stock->setText("");
//    ui->price->setText("");
//    ui->sum->setText("");
//    ui->spinBox->setValue(0);
//    ui->sum->setText("");
//    ui->label_20->setText("");

//}

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

    cout<<"in on_tableView_clicked()";
    onTableSelectChange(1);
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

    //    int r;
    //    r=ui->tableView->currentIndex().row();
    //    //QModelIndex index;

    //    QModelIndex currentIndex;
    //    goods_model->setTable("GOODS");
    //    if(ui->comboBox->currentText()=="全部")
    //    {

    //    }
    //    else
    //    {
    //        goods_model->setFilter(QObject::tr("kind= '%1'").arg(ui->comboBox->currentText()));
    //    }
    //    goods_model->select();//获取表
    //    currentIndex=goods_model->index(r,0);//id
    //    ui->id->setText(goods_model->data(currentIndex).toString());

    //    currentIndex=goods_model->index(r,1);//名称
    //    ui->name->setText(goods_model->data(currentIndex).toString());

    //    currentIndex=goods_model->index(r,2);//库存
    //    ui->stock->setText(goods_model->data(currentIndex).toString());

    //    currentIndex=goods_model->index(r,4);//单价
    //    ui->price->setText(goods_model->data(currentIndex).toString());

    //    QSqlQuery query; //类别
    //    query.exec(QString("select kind from GOODS where goods_id='%1'").arg(ui->id->text()));
    //    query.next();
    //    ui->comboBox->setCurrentText(query.value(0).toString());
    //    //    QSqlQuery query; //类别
    //    //    query.exec(QString("select kind from discounts where discount_id='%1'").arg(ui->id->text()));
    //    //    query.next();
    //    //ui->kind->setCurrentText(query.value(0).toString());

    //    goods_model->setTable("GOODS");
    //    goods_model->setFilter(QObject::tr("kind= '%1'").arg(ui->comboBox->currentText()));
    //    goods_model->select();
    //    ui->tableView->setModel(goods_model);

    //    ui->spinBox->setValue(1);
    //    on_spinBox_valueChanged(1);


}

void MyClient::onTableSelectChange(int row)
{
    cout<<"in onTableSelectChange()";
    int r=1;
    if(row!=0)
        r=ui->tableView->currentIndex().row();

    int id=goods_model->data(goods_model->index(r,0)).toInt();
    cout<<id;

    QSqlQuery query;
    query.prepare("select * from GOODS where goods_id=:goods_id");
    query.bindValue(":goods_id",id);
    query.exec();
    cout<<query.lastQuery();

    if(query.next())
    {
        ui->id->setText(QString::number(id));
        ui->name->setText(query.value("goods_name").toString());
        ui->label_20->setText(query.value("kind").toString());
        ui->stock->setText(query.value("amount").toString());
        ui->price->setText(query.value("price").toString());
        ui->sum->setText(QString::number(ui->spinBox->value()*query.value("price").toInt()));
    }

//    QModelIndex index;

//    if(ui->comboBox->currentText()=="全部")
//    {
//        goods_model->setTable("goods");
//        goods_model->select();
//    }
//    else
//    {
//        goods_model->setTable("goods");
//        goods_model->setFilter(QObject::tr("kind= '%1'").arg(ui->comboBox->currentText()));
//        goods_model->select();
//    }
//    index=goods_model->index(r,0);//id
//    ui->id->setText(goods_model->data(index).toString());

//    index=goods_model->index(r,1);//名称
//    ui->name->setText(goods_model->data(index).toString());
//    index=goods_model->index(r,5);//类别
//    ui->label_20->setText(goods_model->data(index).toString());
//    index=goods_model->index(r,2);//库存
//    ui->stock->setText(goods_model->data(index).toString());
//    //ui->start->setText(goods_model->data(index).toString("yyyy-MM-dd hh:mm:ss"));
//    index=goods_model->index(r,2);//数量
//    ui->spinBox->setValue(1);
//    index=goods_model->index(r,4);//售价
//    ui->price->setText(goods_model->data(index).toString());
//    ui->sum->setText(goods_model->data(index).toString());

//    if(ui->comboBox->currentText()=="全部")
//    {
//        goods_model->setTable("goods");
//        goods_model->select();
//        ui->tableView->setModel(goods_model);
//        return;
//    }

//    goods_model->setTable("goods");
//    goods_model->setFilter(QObject::tr("kind= '%1'").arg(ui->comboBox->currentText()));
//    goods_model->select();
//    ui->tableView->setModel(goods_model);
}
//根据左边种类筛选右侧商品
void MyClient::on_comboBox_currentTextChanged(const QString &arg1)
{
    //    cout<<"in on_comboBox_currentTextChanged()";
    //    goods_model->setTable("GOODS");
    //    if (ui->comboBox->currentText()=="全部")
    //    {
    //        on_pushButton_clicked();
    //        return;
    //    }

    //    goods_model->setFilter(QObject::tr("kind= '%1'").arg(arg1));
    //    goods_model->select();
    //    ui->tableView->setModel(goods_model);


    if(ui->comboBox->currentText()=="全部")
    {
        goods_model->setTable("GOODS");
        goods_model->select();
        ui->tableView->setModel(goods_model);
        return;
    }
    //    k=1;
    cout<<"in on_kind_currentTextChanged()";
    goods_model->setTable("goods");
    goods_model->setFilter(QObject::tr("kind= '%1'").arg(arg1));
    goods_model->select();
    ui->tableView->setModel(goods_model);

}

//显示所有商品

//void MyClient::on_name_currentTextChanged(const QString &arg1)
//{
//    goods_model->setTable("GOODS");
//    goods_model->setFilter(QObject::tr("kind= '%1'").arg(arg1));
//    goods_model->select();
//    ui->tableView->setModel(goods_model);
//}//TODO:搜索功能
////按名字筛选？？？
//void MyClient::on_name_currentTextChanged(const QString &arg1)
//{
//    goods_model->setTable("GOODS");
//    goods_model->setFilter(QObject::tr("goods_name= '%1'").arg(arg1));
//    goods_model->select();
//    ui->tableView->setModel(goods_model);
//}

void MyClient::on_Btn_delete_clicked()
{
    cout<<"click the delete button"<<endl;
    QSqlQuery query;

    QList<QCheckBox*> checkboxList=shoppingWidget->findChildren<QCheckBox*>();
    int goodsId=0;

    for (int i=0;i<checkboxList.length();i++)
    {
        QCheckBox *checkbox=checkboxList.at(i);
        if (checkbox->checkState())//被选中，此时objecName为buy+goodsId
        {
            goodsId=checkbox->objectName().mid(3).toInt();
            query.prepare("delete from SHOPPING_CHARTS where goods_id=:goods_id and customer_id=:customer_id");
            query.bindValue(":goods_id",goodsId);
            query.bindValue(":customer_id",customerId);
            query.exec();
            //            cout<<"in delete from shopping_charts: "<<query.lastError();
        }
    }

    updateShoppingCharts();
}

void MyClient::on_comboBox_activated(const QString &arg1)
{
    ui->id->clear();
    ui->name->clear();
    ui->label_20->clear();
    ui->stock->clear();
    ui->price->clear();
    ui->spinBox->setValue(0);
    ui->sum->clear();

}
