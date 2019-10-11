#ifndef MYCLIENT_H
#define MYCLIENT_H

#include<includes.h>

namespace Ui {
class MyClient;
}

class MyClient : public QDialog
{
    Q_OBJECT

public:
    explicit MyClient(QWidget *parent = nullptr);
    ~MyClient();
    //界面初始化
    void initMyClient();
    QString customerId;
    //商品信息数据网格与表单联动
    void onTableSelectChange(int row);

private slots:
    void on_loginAgain_clicked();

    void on_addShoppingCart_clicked();

    void on_check_clicked();

    void on_charge_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_name_currentTextChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_spinBox_valueChanged(int arg1);

private:
    Ui::MyClient *ui;
    QSqlTableModel *goods_model;
};

#endif // MYCLIENT_H
