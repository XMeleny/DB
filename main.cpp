#include"login.h"
#include"connection.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    createConnection();
    LogIn login;
    login.exec();
}

