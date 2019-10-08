#include"login.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LogIn login;

//if(login.exec()==QDialog::Accepted)
//        {

//        client.show();
//        return a.exec();
//        }
//        else
//            return 0;
    login.exec();
    return a.exec();
}

