#ifndef SHOPPINGCHARTRECORD_H
#define SHOPPINGCHARTRECORD_H

#include <QDialog>

namespace Ui {
class ShoppingChartRecord;
}

class ShoppingChartRecord : public QDialog
{
    Q_OBJECT

public:
    explicit ShoppingChartRecord(QWidget *parent = nullptr);
    ~ShoppingChartRecord();

private:
    Ui::ShoppingChartRecord *ui;
};

#endif // SHOPPINGCHARTRECORD_H
