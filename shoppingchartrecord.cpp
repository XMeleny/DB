#include "shoppingchartrecord.h"
#include "ui_shoppingchartrecord.h"

ShoppingChartRecord::ShoppingChartRecord(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShoppingChartRecord)
{
    ui->setupUi(this);
}

ShoppingChartRecord::~ShoppingChartRecord()
{
    delete ui;
}

void ShoppingChartRecord::on_BtnAdd_clicked()
{

}
