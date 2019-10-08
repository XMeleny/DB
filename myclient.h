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
    //商品信息数据网格与表单联动
//    void onTableSelectChange(int row);

private slots:
    void on_loginAgain_clicked();

    void on_addShoppingCart_clicked();

    void on_check_clicked();

    void on_charge_clicked();

//    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::MyClient *ui;
};

#endif // MYCLIENT_H
