#include"login.h"
#include"connection.h"
#include <QApplication>
#include <addrecordtest.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    createConnection();
    LogIn login;
//    AddRecordTest test;
//    test.exec();

//////if(login.exec()==QDialog::Accepted)
//////        {

//////        client.show();
//////        return a.exec();
//////        }
//////        else
//////            return 0;
    login.exec();
    return a.exec();

    //todo: kill the process after closing the window



}

