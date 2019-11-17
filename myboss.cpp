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
    k=0;
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
//    qDebug()<<target;
    int day1 =date.day();
    int day2 = target.day();
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
    qDebug()<<buffer1<<buffer2;
    orders_model->setTable("ORDERS");
    orders_model->setFilter(QObject::tr("'%1'<=date_format(timing,'%Y-%m-%d') and date_format(timing,'%Y-%m-%d')<='%2'").arg(buffer2).arg(buffer1));
    orders_model->select();
    ui->tableView_2->setModel(orders_model);
    int month = date.month();
    int day = date.day();
    query.prepare("select * from orders where MONTH(timing)=:month and DAY(timing)=:day");
    query.bindValue(":month",month);
    query.bindValue(":day",day);
    sum=sum+query.value("total").toFloat();
    qDebug()<<sum;
    query.exec();
    query.next();
    while (query.next())
    {
        sum=sum+query.value("total").toFloat();
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

            sum=sum+query.value("total").toFloat();
        }

    }
    QString extra = QString("%1").arg(sum);
    QString str = "sum"+extra ;
    //          strings="hello";
    strings = strings.sprintf("%.2f",sum);
    ui->lineEdit->setText("sum: "+strings);
    qDebug()<<"sum: "<<sum;
    ui->tableView_2->setColumnHidden(0,true);
    ui->tableView_2->setColumnHidden(1,true);
    ui->tableView_2->setColumnHidden(2,true);
    ui->tableView_2->setColumnHidden(3,true);
    ui->tableView_2->setColumnHidden(6,true);
    ui->tableView_2->setColumnHidden(7,true);
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
    int y = year.toInt();
    int y_next=y+1;
    QString year2=QString::number(y_next,10);
    QString end;
    int next;
    if(m!=12)
        next = m+1;
    if(m==12)
        next=1;
    //    QDate date = QDate::fromString(begin,"yyyy/M/d");
    QString month2 = QString::number(next,10);
    if(m!=12)
        end = year +"-"+ month2 +"-"+"01";
    else
        end = year2 +"-"+ month2 +"-"+"01";
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

        sum=sum+query.value("total").toFloat();
    }
    qDebug()<<sum;
    QString strings;
    strings = strings.sprintf("%.2f",sum);
    ui->lineEdit->setText("sum:"+strings);
    qDebug()<<"sum: "<<sum;
    ui->tableView_2->setColumnHidden(0,true);
    ui->tableView_2->setColumnHidden(1,true);
    ui->tableView_2->setColumnHidden(2,true);
    ui->tableView_2->setColumnHidden(3,true);
    ui->tableView_2->setColumnHidden(6,true);
    ui->tableView_2->setColumnHidden(7,true);
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

        sum=sum+query.value("total").toFloat();
    }
    qDebug()<<sum;
    QString strings;
    strings = strings.sprintf("%.2f",sum);
    ui->lineEdit->setText("sum: "+strings);
    ui->tableView_2->setColumnHidden(0,true);
    ui->tableView_2->setColumnHidden(1,true);
    ui->tableView_2->setColumnHidden(2,true);
    ui->tableView_2->setColumnHidden(3,true);
    ui->tableView_2->setColumnHidden(6,true);
    ui->tableView_2->setColumnHidden(7,true);//bug
}

void MyBoss::on_addDiscount_clicked()
{
//    cout<<ui->dateEdit->date();
    if(ui->name->text()=="")
    {
        QMessageBox::warning(this,tr("error!!!"),tr("no discount name!"),QMessageBox::Yes);
        return;
    }
    if(ui->comboBox_4->currentText()=="")
    {
        QMessageBox::warning(this,tr("error!!!"),tr("no discount kind!"),QMessageBox::Yes);
        return;
    }
    if(ui->start->text()=="")
    {
        QMessageBox::warning(this,tr("error!!!"),tr("no discount start time!"),QMessageBox::Yes);
        return;
    }
    if(ui->end->text()=="")
    {
        QMessageBox::warning(this,tr("error!!!"),tr("no discount end time!"),QMessageBox::Yes);
        return;
    }
    QSqlQuery query;
    query.prepare("insert into discounts(discount_name,kind,start_time,end_time) "
                  "values(?,?,?,?)");
    query.addBindValue(ui->name->text());
    query.addBindValue(ui->comboBox_4->currentText());
    query.addBindValue(ui->start->date());
    query.addBindValue(ui->end->date());
    query.exec();
    //刷新tableview信息
    discount_model->setTable("discounts");
    discount_model->select();
    ui->tableView->setModel(discount_model);
    QMessageBox::warning(this,tr("success"),tr("successfully add discount!"),QMessageBox::Yes);
}

//删除优惠
void MyBoss::on_deleteDiscount_clicked()
{

    if(createConnection())
    {
        //删除数据库记录
        QSqlQuery query;
        query.exec(QString("delete from discounts where discount_id=%1").arg(ui->id->text()));
        cout<<query.lastError();
        //刷新tableview信息
        ui->name->setText("");
        ui->id->clear();
        ui->start->clear();
        ui->end->clear();

        //        QSqlTableModel *discount_model=new QSqlTableModel;
        discount_model=new QSqlTableModel(this);
        discount_model->setTable("discounts");
        discount_model->select();
        ui->tableView->setModel(discount_model);

        QMessageBox::warning(this,tr("success"),tr("successfully delete discount"),QMessageBox::Yes);
    }

}




void MyBoss::initMyStaff()
{
    QStringList years;
    //显示账单的界面
    ui->toolBox->setCurrentIndex(0);
    //显示账单的界面
    ui->toolBox->setCurrentIndex(0);
    QSqlQuery query;
    discount_model->setTable("discounts");
    discount_model->select();
    ui->tableView->setModel(discount_model);
    query.exec("select distinct YEAR(timing) from orders;");
    while(query.next())
    {
        years.append(query.value(0).toString());
    }
    QCompleter* com=new QCompleter(years,this);
    ui->comboBox_3->clear();
    ui->comboBox_3->addItems(years);
    ui->comboBox_3->setCompleter(com);

    ui->comboBox->clear();
    ui->comboBox->addItems(years);
    ui->comboBox->setCompleter(com);

    QStringList months;
    query.exec("select distinct MONTH(timing) from orders;");//todo: follow the year
    while(query.next())
    {
        months.append(query.value(0).toString());
    }
    ui->comboBox_2->clear();
    ui->comboBox_2->addItems(months);
    ui->comboBox_2->setCompleter(com);
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

//    int id=discount_model->data(discount_model->index(r,0)).toInt();

//    QSqlQuery query;
//    query.prepare("select * from DISCOUNTS where discount_id=:discount_id");
//    query.bindValue(":discount",id);
//    query.exec();

//    if (query.next())
//    {
//        ui->id->setText(QString::number(id));
//        ui->name->setText(query.value("name").toString());
//        ui->start->setDate(query.value("start_time").toDate());
//        ui->end->setDate(query.value("end_time").toDate());
//        ui->comboBox_4->setCurrentText(query.value("kind").toString());
//    }
//    QModelIndex index;

//    if(ui->kind->currentText()=="全部")
//    {
//        discount_model->setTable("discounts");
//        discount_model->select();
//    }
//    else
//    {
//        discount_model->setTable("discounts");
//        discount_model->setFilter(QObject::tr("kind= '%1'").arg(ui->kind->currentText()));
//        discount_model->select();
//    }

//    index=discount_model->index(r,0);//id
//    ui->id->setText(discount_model->data(index).toString());
    int id=discount_model->data(discount_model->index(r,0)).toInt();
    QSqlQuery query;
    query.prepare("select * from DISCOUNTS where discount_id=:discount_id");
    query.bindValue(":discount_id",id);
    query.exec();
    cout<<query.lastQuery()<<query.lastError();

    if(query.next())
    {
        ui->id->setText(QString::number(id));
        ui->name->setText(query.value("discount_name").toString());
        ui->start->setDate(query.value("start_time").toDate());
        ui->end->setDate(query.value("end_time").toDate());
        ui->comboBox_4->setCurrentText(query.value("kind").toString());
    }


//    if(ui->kind->currentText()=="全部")
//    {
//        discount_model->setTable("discounts");
//        discount_model->select();
//        ui->tableView->setModel(discount_model);
//        return;
//    }

//    discount_model->setTable("discounts");
//    discount_model->setFilter(QObject::tr("kind= '%1'").arg(ui->kind->currentText()));
//    discount_model->select();
//    ui->tableView->setModel(discount_model);

}

//下拉框 切换优惠种类的时候
void MyBoss::on_kind_currentTextChanged(const QString &arg1)
{
    ui->id->clear();
    ui->name->clear();
    ui->comboBox_4->setCurrentText("");
    ui->start->clear();
    ui->end->clear();

    if(ui->kind->currentText()=="全部")
    {
        discount_model->setTable("discounts");
        discount_model->select();
        ui->tableView->setModel(discount_model);
        return;
    }

    cout<<"in on_kind_currentTextChanged()";
    discount_model->setTable("discounts");
    discount_model->setFilter(QObject::tr("kind= '%1'").arg(arg1));
    discount_model->select();
    ui->tableView->setModel(discount_model);
}



void MyBoss::on_kind_activated(const QString &arg1)
{
    ui->id->clear();
    ui->name->clear();
    ui->comboBox_4->setCurrentText("");
    ui->start->clear();
    ui->end->clear();
}
