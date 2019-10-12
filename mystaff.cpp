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
//todo:第二次点击 qdebug输出结果为000
void MyStaff::on_tableView_clicked(const QModelIndex &index)
{
    cout<<"in on_tableView_click function"<<endl;
    //    onTableSelectChange(1);
    goods_model->setTable("GOODS");
    goods_model->select();

    QSqlRecord record=goods_model->record(index.row());

    ui->name->setText(record.value("goods_name").toString());
    qDebug()<<record.value("goods_name").toString()<<endl;
    ui->cost->setText(record.value("cost").toString());
    qDebug()<<record.value("cost").toString()<<endl;
    ui->price->setText(record.value("price").toString());
    qDebug()<<record.value("price").toString()<<endl;
    ui->id->setText(record.value("goods_id").toString());
    qDebug()<<record.value("goods_id").toString()<<endl;
    ui->spinBox->setValue(record.value("amount").toInt());
    qDebug()<<record.value("amount").toString()<<endl;
    ui->comboBox->setCurrentText(record.value("kind").toString());
    qDebug()<<record.value("kind").toString()<<endl;
    goods_model->setTable("GOODS");
    goods_model->select();
    ui->tableView->setModel(goods_model);
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

//todo：多次点击会出问题
void MyStaff::on_comboBox_currentTextChanged(const QString &arg1)
{
    goods_model->setTable("GOODS");
    goods_model->setFilter(QObject::tr("kind= '%1'").arg(arg1));
    goods_model->select();
    ui->tableView->setModel(goods_model);
}

void MyStaff::on_pushButton_3_clicked()
{
    goods_model->setTable("GOODS");
    goods_model->select();
    ui->tableView->setModel(goods_model);
}
