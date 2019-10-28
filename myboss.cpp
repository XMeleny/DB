#include "myboss.h"
#include "ui_myboss.h"
#include"login.h"
#include"connection.h"
#include<QDate>
#include <QStandardItem>

MyBoss::MyBoss(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyBoss)
{
    ui->setupUi(this);
    discount_model=new QSqlTableModel;
    connect(ui->comboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(print_s()));
    connect(ui->comboBox_2,SIGNAL(currentIndexChanged(QString)),this,SLOT(print_s()));
    connect(ui->comboBox_3,SIGNAL(currentIndexChanged(QString)),this,SLOT(print_s()));
    initMyStaff();
}

MyBoss::~MyBoss()
{
    delete ui;
}
//重新登录
void MyBoss::on_loginAgain_clicked()
{
    close();
    LogIn login;
    login.exec();

}

//周结账单
void MyBoss::on_weeklyBill_clicked()
{
    orders_model=new QSqlTableModel;
    QString strings;
    QSqlQuery query;
    float sum=0;
    ui->toolBox->setCurrentIndex(0);
    QDate date(QDate::currentDate());
    QDate target = date.addDays(-6);
    qDebug()<<target;
    int day1 =date.day();
    int day2 = date.day();
    QString buffer1,buffer2;
    if(day1<10)
    {
    buffer1 = QString("%1-%2-0%3").arg(date.year()).arg(date.month()).arg(date.day());
    }
    else{

        buffer1 = QString("%1-%2-%3").arg(date.year()).arg(date.month()).arg(date.day());
    }
    if(day2<10)
    {
    buffer2 = QString("%1-%2-0%3").arg(target.year()).arg(target.month()).arg(target.day());
    }
    else{
        buffer2 = QString("%1-%2-%3").arg(target.year()).arg(target.month()).arg(target.day());
    }
        orders_model->setTable("ORDERS");
        orders_model->setFilter(QObject::tr("'%1'<=date_format(timing,'%Y-%m-%d') and date_format(timing,'%Y-%m-%d')<='%2'").arg(buffer2).arg(buffer1));
        orders_model->select();
        ui->tableView_2->setModel(orders_model);
    int month = date.month();
    int day = date.day();
    query.prepare("select * from orders where MONTH(timing)=:month and DAY(timing)=:day");
       query.bindValue(":month",month);
       query.bindValue(":day",day);
       sum=sum+query.value(3).toInt()*query.value(4).toFloat();
       qDebug()<<sum;
       query.exec();
       query.next();
       while (query.next())
      {
          sum=sum+query.value(4).toFloat();
          qDebug()<<query.value(4);
      }
       for(int i=1;i<=6;i++)
        {

                target = date.addDays(-i);

                qDebug()<<target;
                month = target.month();
                day   = target.day();
                query.prepare("select * from orders where MONTH(timing)=:month and DAY(timing)=:day");
                query.bindValue(":month",month);
                query.bindValue(":day",day);
                query.exec();
                while (query.next())
                  {

                       sum=sum+query.value(4).toFloat();
                        }

                   }
           QString extra = QString("%1").arg(sum);
          QString str = "sum"+extra ;
//          strings="hello";
          strings = strings.sprintf("%.2f",sum);
           ui->lineEdit->setText(strings);
           qDebug()<<"sum:"<<sum;
}
//月结账单
void MyBoss::on_monthlyBill_clicked()
{
    orders_model=new QSqlTableModel;
    QComboBox *comboBox = ui->comboBox;
    QComboBox *comboBox_2 = ui->comboBox_2;
    QString year = comboBox->currentText();
    QString month = comboBox_2->currentText();
    qDebug()<<year<<month;
    QString begin = year +"-"+ month+"-" +"01";
    int m = month.toInt();
    int next = m+1;
    QDate date = QDate::fromString(begin,"yyyy/M/d");
    QString month2 = QString::number(next,10);
    QString end = year +"-"+ month2 +"-"+"01";
    qDebug()<<begin<<end;
    orders_model->setTable("ORDERS");
    orders_model->setFilter(QObject::tr("'%1'<=date_format(timing,'%Y-%m-%d') and date_format(timing,'%Y-%m-%d')<'%2'").arg(begin).arg(end));
    orders_model->select();
    ui->tableView_2->setModel(orders_model);
     QSqlQuery query;
     query.prepare("select * from orders where YEAR(timing)=:year and MONTH(timing)=:month");
     query.bindValue(":month",month);
     query.bindValue(":year",year);
     query.exec();
     float sum=0;
     while(query.next())
     {

         sum=sum+query.value(4).toFloat();
     }
     qDebug()<<sum;
     QString strings;
     strings = strings.sprintf("%.2f",sum);
      ui->lineEdit->setText(strings);
      qDebug()<<"sum:"<<sum;
}
//年结账单
void MyBoss::on_yearBill_clicked()
{
    orders_model=new QSqlTableModel;
    QComboBox *comboBox_3 = ui->comboBox_3;
    QString year = comboBox_3->currentText();
    QString begin = year +"-"+"1"+"-"+"01";
    QString end =year +"-"+"12"+"-"+"31";
    orders_model->setTable("ORDERS");
    orders_model->setFilter(QObject::tr("'%1'<=date_format(timing,'%Y-%m-%d') and date_format(timing,'%Y-%m-%d')<'%2'").arg(begin).arg(end));
    orders_model->select();
     ui->tableView_2->setModel(orders_model);
     QSqlQuery query;
     query.prepare("select * from orders where YEAR(timing)=:year");
     query.bindValue(":year",year);
     query.exec();
     float sum=0;
     while(query.next())
     {

         sum=sum+query.value(4).toFloat();
     }
     qDebug()<<sum;
     QString strings;
     strings = strings.sprintf("%.2f",sum);
      ui->lineEdit->setText(strings);
}

void MyBoss::on_addDiscount_clicked()
{

    QSqlQuery query;
    query.prepare("insert into discounts(discount_id,discount_name,kind,start_time,end_time) "
                  "values(?,?,?,?,?)");
    query.addBindValue(ui->id->text());
    query.addBindValue(ui->name->text());
    query.addBindValue(ui->kind->currentText());
    query.addBindValue(ui->start->text());
    query.addBindValue(ui->end->text());
    query.exec();

    //刷新tableview信息
    discount_model->setTable("discounts");
    discount_model->select();
    ui->tableView->setModel(discount_model);
}

//删除优惠
void MyBoss::on_deleteDiscount_clicked()
{

    if(createConnection())
    {
        //删除数据库记录
        QSqlQuery query;
        query.exec(QString("delete from discounts where discount_id=%1").arg(ui->id->text()));
        //刷新tableview信息
        ui->name->setText("");
        ui->id->setText("");
        ui->start->setText("");
        ui->end->setText("");

        //        QSqlTableModel *discount_model=new QSqlTableModel;
        discount_model=new QSqlTableModel(this);
        discount_model->setTable("discounts");
        discount_model->select();
        ui->tableView->setModel(discount_model);

    }

}




void MyBoss::initMyStaff()
{
    //    if(createConnection())
    //    {
    QStringList strings;

    //显示账单的界面
    ui->toolBox->setCurrentIndex(0);
    QStringList month;
         month << "1" << "2" << "3" << "4" << "5"<<"6"<<"7"<<"8"<<"9"<<"10"<<"11"<<"12";
     ui->comboBox_2->addItems(month);
     QStringList year;
      year<<"2017"<<"2018"<<"2019";
      ui->comboBox->addItems(year);
      QStringList year_2;
      year_2<<"2017"<<"2018"<<"2019";
      ui->comboBox_3->addItems(year_2);
    //显示账单的界面
    ui->toolBox->setCurrentIndex(0);
    QSqlQuery query;


    //将商品类型显示到combobox
    //        query.exec("select kind from GOODS;");
    //        while(query.next())
    //        {
    //            strings.append(query.value(0).toString());
    //        }
    //        QCompleter* com=new QCompleter(strings,this);
    //        ui->kind->clear();
    //        ui->kind->addItems(strings);
    //        ui->kind->setCompleter(com);


    //        将商品详情显示到tableView
    discount_model->setTable("discounts");
    discount_model->select();
    ui->tableView->setModel(discount_model);

}


void MyBoss::on_tableView_clicked(const QModelIndex &index)
{
    cout<<"in on_tableView_clicked()";
    onTableSelectChange(1);

}


void MyBoss::onTableSelectChange(int row)
{
    cout<<"in onTableSelectChange()";
    int r=1;
    if(row!=0)
        r=ui->tableView->currentIndex().row();
    QModelIndex index;

    discount_model->setTable("discounts");
    discount_model->select();//获取表
    index=discount_model->index(r,0);//id
    ui->id->setText(discount_model->data(index).toString());

    index=discount_model->index(r,1);//名称
    ui->name->setText(discount_model->data(index).toString());
    index=discount_model->index(r,3);//开始时间
    ui->start->setText(discount_model->data(index).toString());
    //ui->start->setText(discount_model->data(index).toString("yyyy-MM-dd hh:mm:ss"));
    index=discount_model->index(r,4);//结束时间
    ui->end->setText(discount_model->data(index).toString());
    //ui->end->setText(discount_model->data(index).toString("yyyy-MM-dd hh:mm:ss"));
    QSqlQuery query; //类别
    query.exec(QString("select kind from discounts where discount_id='%1'").arg(ui->id->text()));
    query.next();
    ui->kind->setCurrentText(query.value(0).toString());

    discount_model->setTable("discounts");
    discount_model->select();
    ui->tableView->setModel(discount_model);

}

//下拉框 切换优惠种类的时候
void MyBoss::on_kind_currentTextChanged(const QString &arg1)
{

    cout<<"in on_kind_currentTextChanged()";
    discount_model->setTable("discounts");
    discount_model->setFilter(QObject::tr("kind= '%1'").arg(arg1));
    discount_model->select();
    ui->tableView->setModel(discount_model);
}

//点击按钮 显示所有优惠
void MyBoss::on_pushButton_clicked()
{

    cout<<"in on_pushButton_clicked()";
    discount_model->setTable("discounts");
    discount_model->select();
    ui->tableView->setModel(discount_model);
}
