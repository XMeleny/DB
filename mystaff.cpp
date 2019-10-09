#include "mystaff.h"
#include "ui_mystaff.h"
#include "login.h"
#include "connection.h"


MyStaff::MyStaff(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyStaff)
{
    ui->setupUi(this);
    goods_model=new QSqlTableModel;
    initMyStaff();   //初始化
}

MyStaff::~MyStaff()
{
    delete ui;
}

void MyStaff::on_loginAgain_clicked()
{
    //重新登录
    close();
    LogIn login;
    login.exec();
}

void MyStaff::initMyStaff()
{
    //if(createConnection())
    //{
        QStringList strings;
        ui->toolBox->setCurrentIndex(0);
        QSqlQuery query;


        //将商品类型显示到comboBox
        query.exec("select kind from GOODS;");
        while(query.next())
        {
            strings.append(query.value(0).toString());
        }
        QCompleter* com=new QCompleter(strings,this);
        ui->comboBox->clear();
        ui->comboBox->addItems(strings);
        ui->comboBox->setCompleter(com);


        //将商品详情显示到tableView
        goods_model->setTable("GOODS");
        goods_model->select();
        ui->tableView->setModel(goods_model);




        goods_model=new QSqlTableModel;
        goods_model->setTable("GOODS");
        goods_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
        goods_model->select(); //选取整个表的所有行

        ui->tableView_2->setModel(goods_model);
        ui->tableView_2->hideColumn(0);//hide the id,in case the changes
        ui->tableView_2->hideColumn(2);//hide the amount
        ui->tableView_2->hideColumn(3);//hide the cost
   // }

}


//点击右侧商品，左边修改
void MyStaff::on_tableView_clicked(const QModelIndex &index)
{
    onTableSelectChange(1);
}
void MyStaff::onTableSelectChange(int row)
{
    int r=1;
    if(row!=0)
        r=ui->tableView->currentIndex().row();
    QModelIndex index;
//    QSqlTableModel *goods_model=new QSqlTableModel(this);
    goods_model->setTable("GOODS");
    goods_model->select();
    index=goods_model->index(r,1);//名称
    ui->name->setText(goods_model->data(index).toString());
    index=goods_model->index(r,3);//进价
    ui->cost->setText(goods_model->data(index).toString());
    index=goods_model->index(r,4);//售价
    ui->price->setText(goods_model->data(index).toString());
    index=goods_model->index(r,0);//id
    ui->id->setText(goods_model->data(index).toString());
    index=goods_model->index(r,2);//数量
    int x=goods_model->data(index).toInt();
    ui->spinBox->setValue(x);

    QSqlQuery query; //类别
    query.exec(QString("select kind from GOODS where goods_name='%1'").arg(ui->name->text()));
    query.next();
    ui->comboBox->setCurrentText(query.value(0).toString());

}


//入库操作
void MyStaff::on_putIn_clicked()
{
    if(createConnection())
    {
        QSqlQuery query;
        QString name1=ui->name->text();
        QString kind1=ui->comboBox->currentText();
        double cost1=ui->cost->text().toDouble();
        double price1=ui->price->text().toDouble();
        int amount1=ui->spinBox->value();
        int id1=ui->id->text().toInt();
        //插入不成功
        QSqlQueryModel model;
        //        model.setQuery("insert into goods(goods_id,goods_name,amount,cost,price,kind)values(3,'2',3,4,5,'6')");
        //        if (model.lastError().isValid())
        //            qDebug() << model.lastError();
        //        qDebug()<<id1<<" "<<name1<<" "<<amount1<<" "<<cost1<<" "<<price1<<" "<<kind1;

        query.prepare("insert into goods(goods_id,goods_name,amount,cost,price,kind) values(:id,:name,:amount,:cost,:price,:kind)");
        query.bindValue(":id",id1);
        query.bindValue(":name",name1);
        query.bindValue(":amount",amount1);
        query.bindValue(":cost",cost1);
        query.bindValue(":price",price1);
        query.bindValue(":kind",kind1);

        query.exec();


        if (model.lastError().isValid())
            qDebug() << model.lastError();

        //刷新tableview信息
//        QSqlTableModel *goods_model=new QSqlTableModel(this);
        goods_model->setTable("GOODS");
        goods_model->select();
        ui->tableView->setModel(goods_model);
    }
}

//清仓操作
void MyStaff::on_putOut_clicked()
{
    if(createConnection())
    {
        //删除数据库记录
        QSqlQuery query;
        query.exec(QString("delete from GOODS where goods_id=%1").arg(ui->id->text()));
        //刷新tableview信息
        ui->name->setText("");
        ui->cost->setText("");
        ui->price->setText("");
        ui->spinBox->setValue(0);
        ui->id->setText("");

//        QSqlTableModel *goods_model=new QSqlTableModel;
        goods_model=new QSqlTableModel(this);
        goods_model->setTable("GOODS");
        goods_model->select();
        ui->tableView->setModel(goods_model);

    }
}

void MyStaff::on_pushButton_2_clicked()
{
    goods_model->database().transaction(); //开始事务操作
    if (goods_model->submitAll())
    {
        goods_model->database().commit(); //提交

//        QSqlTableModel *goods_model=new QSqlTableModel;
        goods_model->setTable("GOODS");
        goods_model->select();
        ui->tableView->setModel(goods_model);


    } else
    {
        goods_model->database().rollback(); //回滚
        QMessageBox::warning(this, tr("tableModel"), tr("数据库错误: %1").arg(goods_model->lastError().text()));
    }
}
