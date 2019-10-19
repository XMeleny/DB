#ifndef CONNECTION_H
#define CONNECTION_H
#include<includes.h>
static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");


//    db.setHostName("116.56.158.101");//todo
//    db.setPort(3306);
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
        cout<<db.lastError();
        return true;
    }

}
#endif // CONNECTION_H
