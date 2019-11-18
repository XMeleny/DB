#ifndef MYBOSS_H
#define MYBOSS_H

#include<includes.h>

namespace Ui {
class MyBoss;
}

class MyBoss : public QDialog
{
    Q_OBJECT

public:
    explicit MyBoss(QWidget *parent = nullptr);
    ~MyBoss();
    //界面初始化
    void initMyStaff();
    //商品信息数据网格与表单联动
    void onTableSelectChange(int row);

private slots:
    void on_loginAgain_clicked();

    void on_weeklyBill_clicked();

    void on_monthlyBill_clicked();

    void on_yearBill_clicked();

    void on_addDiscount_clicked();

    void on_deleteDiscount_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_kind_currentTextChanged(const QString &arg1);

    void on_kind_activated(const QString &arg1);

private:
    Ui::MyBoss *ui;
    QSqlTableModel *discount_model;
    QSqlTableModel *orders_model;
    int k;
};

#endif // MYBOSS_H
