#ifndef CONNECTION_H
#define CONNECTION_H
#include<includes.h>
static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

    //连接远程
//    db.setHostName("118.89.44.161");
//    db.setPort(3306);
//    db.setDatabaseName("test");
//    db.setUserName("xm");
//    db.setPassword("xm123456");

    //连接本地
    db.setDatabaseName("test");
    db.setUserName("root");
    db.setPassword("123456");


    //打开数据库
    if(!db.open())
    {
        cout<<"linking database error!!!";
        cout<<db.lastError();
        return false;
    }
    else
    {
        cout<<"linking database success!!!";
        return true;
    }

}
#endif // CONNECTION_H
