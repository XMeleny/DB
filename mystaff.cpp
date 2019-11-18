#include "mystaff.h"
#include "ui_mystaff.h"
#include "login.h"
#include "connection.h"

/*
 * 构造函数
 * 执行初始化函数
*/
MyStaff::MyStaff(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyStaff)
{
    ui->setupUi(this);
    goods_model=new QSqlTableModel;
    initMyStaff();
}

/*
 * 析构函数
*/
MyStaff::~MyStaff()
{
    delete ui;
}

/*
 * 重新登录
 * 退出当前窗口
 * 打开登录界面
*/
void MyStaff::on_loginAgain_clicked()
{
    //重新登录
    close();
    LogIn login;
    login.exec();
}

/*
 * 初始化函数
 * 将商品显示到tableView中
*/
void MyStaff::initMyStaff()
{
    ui->toolBox->setCurrentIndex(0);
    QSqlQuery query;
    //将商品类型显示到comboBox
    QStringList strings1;
    strings1.append("全部");
    //将商品类型显示到comboBox，且不重复，测试通过
    query.exec("select distinct kind from GOODS;");
    cout<<query.lastError();
    while(query.next())
    {
        strings1.append(query.value(0).toString());
    }

    QCompleter* com=new QCompleter(strings1,this);
    ui->comboBox->clear();
    ui->comboBox->addItems(strings1);
    ui->comboBox->setCompleter(com);


    //将商品详情显示到tableView
    goods_model->setTable("GOODS");
    goods_model->select();
    ui->tableView->setModel(goods_model);

    goods_model->setTable("GOODS");
    goods_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    goods_model->select(); //选取整个表的所有行

    ui->tableView_2->setModel(goods_model);
    ui->tableView_2->setColumnHidden(0,true);
    ui->tableView_2->setColumnHidden(5,true);


}


/*
 * 点击商品显示的tableView
 * 把数据放到左边的详细信息栏中
 * 使用onTableSelectChange
*/
void MyStaff::on_tableView_clicked(const QModelIndex &index)
{
    cout<<"in on_tableView_clicked()";
    onTableSelectChange(1);
}


/*
 * 当点击右边不同的商品时
 * 响应，修改详细信息栏中的信息
*/
void MyStaff::onTableSelectChange(int row)
{
    cout<<"in onTableSelectChange()";

    int r=1;
    if(row!=0)
        r=ui->tableView->currentIndex().row();

    int id=goods_model->data(goods_model->index(r,0)).toInt();

    QSqlQuery query;
    query.prepare("select * from GOODS where goods_id=:goods_id");
    query.bindValue(":goods_id",id);
    query.exec();

    if (query.next())
    {
        ui->id->setText(QString::number(id));
        ui->name->setText(query.value("goods_name").toString());
        ui->cost->setText(query.value("cost").toString());
        ui->price->setText(query.value("price").toString());
        ui->amount->setText(query.value("amount").toString());
        ui->lineEdit_2->setText(query.value("kind").toString());
    }
}


/*
 * 入库操作
*/
void MyStaff::on_putIn_clicked()
{
    QSqlQuery query;
    QString name1=ui->name->text();
    QString kind1=ui->lineEdit_2->text();
    double cost1=ui->cost->text().toDouble();
    double price1=ui->price->text().toDouble();
    int amount1=ui->amount->text().toInt();
    cout<<name1<<kind1<<cost1<<price1<<amount1;

    //插入不成功
    QSqlQueryModel model;

    query.prepare("insert into goods(goods_name,amount,cost,price,kind) "
                  "values(:name,:amount,:cost,:price,:kind)");
    query.bindValue(":name",name1);
    query.bindValue(":amount",amount1);
    query.bindValue(":cost",cost1);
    query.bindValue(":price",price1);
    query.bindValue(":kind",kind1);

    query.exec();
    cout<<query.lastError();
    cout<<query.lastQuery();


    if (model.lastError().isValid())
        qDebug() << model.lastError();

    //刷新tableview信息
    goods_model->setTable("GOODS");
    goods_model->select();
    ui->tableView->setModel(goods_model);

    //将商品类型显示到comboBox
    QStringList kinds;
    kinds.append("全部");
    //将商品类型显示到comboBox，且不重复，测试通过
    query.exec("select distinct kind from GOODS;");
    cout<<query.lastError();
    while(query.next()){
        kinds.append(query.value(0).toString());
    }
    QCompleter* com=new QCompleter(kinds,this);
    ui->comboBox->clear();
    ui->comboBox->addItems(kinds);
    ui->comboBox->setCompleter(com);

    goods_model->setTable("GOODS");
    goods_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    goods_model->select(); //选取整个表的所有行
    ui->tableView_2->setModel(goods_model);

    ui->tableView_2->setColumnHidden(0,true);
    ui->tableView_2->setColumnHidden(5,true);

}


/*
 * 清仓操作
*/
void MyStaff::on_putOut_clicked()
{
    //删除数据库记录
    QSqlQuery query;
    query.exec(QString("delete from GOODS where goods_id=%1").arg(ui->id->text()));
    //刷新tableview信息
    ui->name->setText("");
    ui->cost->setText("");
    ui->price->setText("");
    ui->amount->setText(0);
    ui->id->setText("");
    ui->lineEdit_2->clear();

    goods_model=new QSqlTableModel(this);
    goods_model->setTable("GOODS");
    goods_model->select();
    ui->tableView->setModel(goods_model);

    //将商品类型显示到comboBox
    QStringList kinds;
    kinds.append("全部");
    //将商品类型显示到comboBox，且不重复，测试通过
    query.exec("select distinct kind from GOODS;");
    cout<<query.lastError();
    while(query.next()){
        kinds.append(query.value(0).toString());
    }
    QCompleter* com=new QCompleter(kinds,this);
    ui->comboBox->clear();
    ui->comboBox->addItems(kinds);
    ui->comboBox->setCompleter(com);

    goods_model->setTable("GOODS");
    goods_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    goods_model->select(); //选取整个表的所有行
    ui->tableView_2->setModel(goods_model);

    ui->tableView_2->setColumnHidden(0,true);
    ui->tableView_2->setColumnHidden(5,true);
}


/*
 * 开始事务操作
*/
void MyStaff::on_pushButton_2_clicked()
{
    goods_model->database().transaction(); //开始事务操作
    if (goods_model->submitAll())
    {
        goods_model->database().commit(); //提交

        goods_model->setTable("GOODS");
        goods_model->select();
        ui->tableView->setModel(goods_model);
    }
    else
    {
        goods_model->database().rollback(); //回滚
        QMessageBox::warning(this, tr("tableModel"), tr("数据库错误: %1").arg(goods_model->lastError().text()));
    }
}

/*
 * 选择显示的商品类别时
 * 修改商品显示总表的信息
 * 显示选择的部分商品
*/
void MyStaff::on_comboBox_currentTextChanged(const QString &arg1)
{

    if(ui->comboBox->currentText()=="全部")
    {
        goods_model->setTable("GOODS");
        goods_model->select();
        ui->tableView->setModel(goods_model);
        return;
    }
    cout<<"in on_kind_currentTextChanged()";
    goods_model->setTable("goods");
    goods_model->setFilter(QObject::tr("kind= '%1'").arg(arg1));
    goods_model->select();
    ui->tableView->setModel(goods_model);
}


/*
 * 当商品类别选择框点击时
 * 清空详细信息栏的所有数据
*/
void MyStaff::on_comboBox_activated(const QString &arg1)
{
    ui->id->clear();
    ui->name->clear();
    ui->cost->clear();
    ui->price->clear();
    ui->amount->clear();
    ui->lineEdit_2->clear();
}
