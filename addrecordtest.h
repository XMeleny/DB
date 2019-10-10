#ifndef ADDRECORDTEST_H
#define ADDRECORDTEST_H

#include <QDialog>

namespace Ui {
class AddRecordTest;
}

class AddRecordTest : public QDialog
{
    Q_OBJECT

public:
    explicit AddRecordTest(QWidget *parent = nullptr);
    ~AddRecordTest();

private slots:
    void on_pushButton_clicked();

private:
    Ui::AddRecordTest *ui;
};

#endif // ADDRECORDTEST_H
