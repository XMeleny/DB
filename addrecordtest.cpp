#include "addrecordtest.h"
#include "ui_addrecordtest.h"
#include <includes.h>

AddRecordTest::AddRecordTest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddRecordTest)
{
    ui->setupUi(this);
}

AddRecordTest::~AddRecordTest()
{
    delete ui;
}

void AddRecordTest::on_pushButton_clicked()
{
    QHBoxLayout *horizontalLayout=new QHBoxLayout;
    QCheckBox *buy=new QCheckBox("");
    QLabel *goodsName=new QLabel("goodsName");
    horizontalLayout->addWidget(buy);
    horizontalLayout->addWidget(goodsName);

    ui->verticalLayout->addLayout(horizontalLayout);

}
