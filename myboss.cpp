#include "myboss.h"
#include "ui_myboss.h"
#include"login.h"
#include"connection.h"


MyBoss::MyBoss(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyBoss)
{
    ui->setupUi(this);
    discount_model=new QSqlTableModel;
    initMyStaff();
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
//todo:添加优惠不成功
void MyBoss::on_addDiscount_clicked()
{

    if(createConnection())
    {
        QDateTime stime,etime;
        QSqlQuery query;
        QString name1=ui->name->text();
        QString kind1=ui->kind->currentText();
        QString start=ui->start->text();
        QString end=ui->end->text();
        stime=QDateTime::fromString(start, "yyyy-MM-dd hh:mm:ss");
        etime=QDateTime::fromString(end, "yyyy-MM-dd hh:mm:ss");
        int id1=ui->id->text().toInt();
        QSqlQueryModel model;

        query.prepare("insert into discounts(discount_id,dicount_name,kind,start_time,end_time) values(:id,:name,:kind,:start,:end)");
        query.bindValue(":id",id1);
        query.bindValue(":name",name1);
        query.bindValue(":kind",kind1);
        query.bindValue(":start",stime);
        query.bindValue(":end",etime);

        query.exec();


        if (model.lastError().isValid())
            qDebug() << model.lastError();

        //刷新tableview信息
        discount_model->setTable("discounts");
        discount_model->select();
        ui->tableView->setModel(discount_model);

    }
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

    //    }

}
void MyBoss::on_tableView_clicked(const QModelIndex &index)
{
    onTableSelectChange(1);
}

//todo:时间显示有点问题
void MyBoss::onTableSelectChange(int row)
{
    int r=1;
    if(row!=0)
        r=ui->tableView->currentIndex().row();
    QModelIndex index;
    //    QSqlTableModel *discount_model=new QSqlTableModel(this);
    discount_model->setTable("discounts");
    discount_model->select();
    index=discount_model->index(r,0);//id
    ui->id->setText(discount_model->data(index).toString());
    index=discount_model->index(r,1);//名称
    ui->name->setText(discount_model->data(index).toString());
    index=discount_model->index(r,3);//开始时间
    ui->start->setText(discount_model->data(index).toString());
    //    ui->start->setText(discount_model->data(index).toString("yyyy-MM-dd hh:mm:ss"));
    index=discount_model->index(r,4);//结束时间
    ui->end->setText(discount_model->data(index).toString());
    //    ui->end->setText(discount_model->data(index).toString("yyyy-MM-dd hh:mm:ss"));
    QSqlQuery query; //类别
    query.exec(QString("select kind from discounts where discount_id='%1'").arg(ui->id->text()));
    query.next();
    ui->kind->setCurrentText(query.value(0).toString());

}

void MyBoss::on_kind_currentTextChanged(const QString &arg1)
{
    discount_model->setTable("discounts");
    discount_model->setFilter(QObject::tr("kind= '%1'").arg(arg1));
    discount_model->select();
    ui->tableView->setModel(discount_model);
}

//显示所有商品
void MyBoss::on_pushButton_clicked()
{
    //    //if(createConnection())
    //    //{

    //    QSqlQuery query;
    //    query.exec("select *from GOODS;");
    //    while(query.next())
    //    {
    //        qDebug()<<query.value(0).toInt()<<query.value(1).toString();
    //    }
    //    //}

    discount_model->setTable("discounts");
    discount_model->select();
    ui->tableView->setModel(discount_model);
}
