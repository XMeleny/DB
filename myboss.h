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

private slots:
    void on_loginAgain_clicked();

    void on_pushButton_clicked();

    void on_weeklyBill_clicked();

    void on_monthlyBill_clicked();

    void on_yearBill_clicked();

    void on_addDiscount_clicked();

    void on_deleteDiscount_clicked();

private:
    Ui::MyBoss *ui;
};

#endif // MYBOSS_H
