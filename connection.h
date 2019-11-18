#ifndef CONNECTION_H
#define CONNECTION_H
#include<includes.h>
static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

    db.setHostName("118.89.44.161");
    db.setPort(3306);
    db.setDatabaseName("test");
    db.setUserName("xm");
    db.setPassword("xm123456");
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
