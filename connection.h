#ifndef CONNECTION_H
#define CONNECTION_H
#include<includes.h>
static bool createConnection()
{
QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

    db.setDatabaseName("test");    //使用哪个数据库
    db.setUserName("root");
    db.setPassword("123456");
    //打开数据库
    if(!db.open())
    {
        cout<<"linking database error!!!";
        return false;
    }
    else
    {
        cout<<"linking database success!!!";
        return true;
    }

}
#endif // CONNECTION_H
