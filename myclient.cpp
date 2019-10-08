#include "myclient.h"
#include "ui_myclient.h"
#include"login.h"
#include"connection.h"


MyClient::MyClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyClient)
{

    ui->setupUi(this);
     qDebug()<<"hhh";
    void initMyClient();
}

MyClient::~MyClient()
{
    delete ui;
}

void MyClient::on_loginAgain_clicked()
{
    close();
LogIn login;
login.exec();
}

//界面初始化
void MyClient::initMyClient()
{
    qDebug()<<"hhh";
    if(createConnection())
    {
        qDebug()<<"hhh";
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
//        QSqlQueryModel *goodsKind=new QSqlQueryModel(this);
//        goodsKind->setQuery("selec kind from GOODS;");
//        this->ui->comboBox->setModel(goodsKind);


        //将商品详情显示到tableView
        QSqlTableModel *goods_model=new QSqlTableModel(this);
        goods_model->setTable("GOODS");
        goods_model->select();
        ui->tableView->setModel(goods_model);
    }
}






//点击右侧商品，左边修改
//void MyClient::on_tableView_clicked(const QModelIndex &index)
//{
//    onTableSelectChange(1);
//}

//void MyClient::onTableSelectChange(int row)
//{
//    int r=1;
//    if(row!=0)
//        r=ui->tableView->currentIndex().row();
//    QModelIndex index;
//    QSqlTableModel *goods_model=new QSqlTableModel(this);
//    goods_model->setTable("GOODS");
//    goods_model->select();
//    index=goods_model->index(r,4);//售价
//    ui->price->setText(goods_model->data(index).toString());
//    index=goods_model->index(r,2);//库存
//    ui->stock->setText(goods_model->data(index).toString());
//    index=goods_model->index(r,4);//总价
//    ui->price->setText(goods_model->data(index).toString());
//    QSqlQuery query; //类别
//    query.exec(QString("select kind from GOODS where goods_name='%1'").arg(ui->name->text()));
//    query.next();
//    ui->comboBox->setCurrentText(query.value(0).toString());

//}

//加入购物车--添加复选框
void MyClient::on_addShoppingCart_clicked()
{

}

//下单--清空复选框--扣钱
void MyClient::on_check_clicked()
{

}

//充值
void MyClient::on_charge_clicked()
{

}


