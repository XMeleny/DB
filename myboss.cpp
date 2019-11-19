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
//    connect(ui->comboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(print_s()));
//    connect(ui->comboBox_2,SIGNAL(currentIndexChanged(QString)),this,SLOT(print_s()));
//    connect(ui->comboBox_3,SIGNAL(currentIndexChanged(QString)),this,SLOT(print_s()));
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
    QSqlQuery query;
    float sum=0;

    QDate endDay=QDate::currentDate();
    QDate startDay=endDay.addDays(-6);

    QString strEndDay=endDay.toString("yyyy-MM-dd");
    QString strStartDay=startDay.toString("yyyy-MM-dd");
    cout<<"endday: "<<endDay<<"strEndDay: "<<strEndDay;

    //handle the sum
    query.prepare("select sum(total) from ORDERS where :startDay<=timing and timing<=:endDay");
    query.bindValue(":startDay",startDay);
    query.bindValue(":endDay",endDay);
    query.exec();
    if(query.next())
    {
        sum+=query.value(0).toFloat();
    }
    cout<<"sum is: "<<sum;

    //handle the tableView
    //set the table
    orders_model=new QSqlTableModel;
    orders_model->setTable("ORDERS");
    orders_model->setFilter(QObject::tr("'%1'<=date_format(timing,'%Y-%m-%d') and date_format(timing,'%Y-%m-%d')<='%2'").arg(strStartDay).arg(strEndDay));
    orders_model->select();
    ui->tableView_2->setModel(orders_model);
    //set the sum
    ui->lineEdit->setText("sum: "+QString::number(sum));

    //left total and timing
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
    int year=ui->spinBox->value();
    int month=ui->spinBox_2->value();

    QSqlQuery query;
    float sum=0;

    query.prepare("select sum(total) from ORDERS where YEAR(timing)=:year and MONTH(timing)=:month");
    query.bindValue(":year",year);
    query.bindValue(":month",month);
    query.exec();
    if(query.next())
    {
        sum=query.value(0).toFloat();
    }
    cout<<sum;

    orders_model=new QSqlTableModel;
    orders_model->setTable("ORDERS");
    orders_model->setFilter(QObject::tr("%1=YEAR(timing) and MONTH(timing)=%2").arg(year).arg(month));
    orders_model->select();
    ui->tableView_2->setModel(orders_model);

    ui->tableView_2->setColumnHidden(0,true);
    ui->tableView_2->setColumnHidden(1,true);
    ui->tableView_2->setColumnHidden(2,true);
    ui->tableView_2->setColumnHidden(3,true);
    ui->tableView_2->setColumnHidden(6,true);
    ui->tableView_2->setColumnHidden(7,true);

    ui->lineEdit->setText("sum: "+QString::number(sum));

}


//年结账单
void MyBoss::on_yearBill_clicked()
{
    //from xm
    QSqlQuery query;
    float sum=0;

    int year=ui->spinBox_3->value();
    query.prepare("select sum(total) from ORDERS where YEAR(timing)=:year");
    query.bindValue(":year",year);
    query.exec();
    if(query.next())
    {
        sum=query.value(0).toFloat();
    }
    cout<<"in year sum:"<<sum;

    orders_model=new QSqlTableModel;
    orders_model->setTable("ORDERS");
    orders_model->setFilter(QObject::tr("%1=YEAR(timing)").arg(year));
    orders_model->select();
    ui->tableView_2->setModel(orders_model);

    ui->tableView_2->setColumnHidden(0,true);
    ui->tableView_2->setColumnHidden(1,true);
    ui->tableView_2->setColumnHidden(2,true);
    ui->tableView_2->setColumnHidden(3,true);
    ui->tableView_2->setColumnHidden(6,true);
    ui->tableView_2->setColumnHidden(7,true);

    ui->lineEdit->setText("sum: "+QString::number(sum));

}


void MyBoss::on_addDiscount_clicked()
{
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
    query.prepare("insert into DISCOUNTS(discount_name,kind,start_time,end_time) "
                  "values(?,?,?,?)");
    query.addBindValue(ui->name->text());
    query.addBindValue(ui->comboBox_4->currentText());
    query.addBindValue(ui->start->date());
    query.addBindValue(ui->end->date());
    query.exec();
    cout<<query.lastQuery()<<query.lastError();

    //刷新tableview信息
    discount_model->setTable("DISCOUNTS");
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
        query.exec(QString("delete from DISCOUNTS where discount_id=%1").arg(ui->id->text()));
        cout<<query.lastError();
        //刷新tableview信息
        ui->name->setText("");
        ui->id->clear();
        ui->start->clear();
        ui->end->clear();

        discount_model=new QSqlTableModel(this);
        discount_model->setTable("DISCOUNTS");
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

    discount_model->setTable("DISCOUNTS");
    discount_model->select();
    ui->tableView->setModel(discount_model);

    QDate date=QDate::currentDate();
    int year=date.year();
    int month=date.month();
    cout<<year<<month;

    ui->spinBox->setMinimum(2000);
    ui->spinBox->setMaximum(year);

    ui->spinBox_2->setMinimum(1);
    ui->spinBox_2->setMaximum(12);

    ui->spinBox_3->setMinimum(2000);
    ui->spinBox_3->setMaximum(year);

    ui->spinBox->setValue(year);
    ui->spinBox_2->setValue(month);
    ui->spinBox_3->setValue(year);

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
        discount_model->setTable("DISCOUNTS");
        discount_model->select();
        ui->tableView->setModel(discount_model);
        return;
    }

    cout<<"in on_kind_currentTextChanged()";
    discount_model->setTable("DISCOUNTS");
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
