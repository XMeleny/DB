#ifndef MYSTAFF_H
#define MYSTAFF_H

#include<includes.h>

namespace Ui {
class MyStaff;
}

class MyStaff : public QDialog
{
    Q_OBJECT

public:
    explicit MyStaff(QWidget *parent = nullptr);
    ~MyStaff();
    //界面初始化
    void initMyStaff();
    //商品信息数据网格与表单联动
    void onTableSelectChange(int row);
    //显示商品样照
    void showGoodsPhoto();
    //加载“预售订单”页商品名称列表
    void loadPreGoods();
    //“预售订单”页商品名与表单联动
    void onPreNameComboBoxChange();

private slots:
    void on_loginAgain_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_putIn_clicked();

    void on_putOut_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MyStaff *ui;
    QSqlTableModel *goods_model;
};

#endif // MYSTAFF_H
