#ifndef INCLUDES_H
#define INCLUDES_H

#define cout qDebug() << "[" <<__FILE__<<":"<<__LINE__<<"]"

//////////////////////////////////qt变量相关//////////////////////////////////
#include <QDebug>
#include<QString>
#include<QVariant>
#include <QCompleter>
#include <windows.h>
#include <QTextCodec>
#include <QColor>
#include<QTime>
#include <QTextCodec>
////////////////////////////////////////////////////////////////////////////


//////////////////////////////////数据库相关//////////////////////////////////
#include <QtSql/QSqlDatabase>
#include <QtSql/QSql>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include<QtSql/QSqlQueryModel>
#include<QtSql/QSqlTableModel>
#include<QSqlTableModel>
#include<QSqlDriver>
#include<QSqlQueryModel>
#include<QSqlTableModel>
#include <QSqlQuery>
#include<QSqlRecord>
#include<QSqlError>
////////////////////////////////////////////////////////////////////////////


//////////////////////////////////组件相关//////////////////////////////////
#include <QMessageBox>
#include <QDialog>
#include"QPushButton"
#include<QLineEdit>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include <QLabel>
#include<QCheckBox>
#include<QPushButton>
#include <QApplication>
#include <QTableView>
////////////////////////////////////////////////////////////////////////////
#endif // INCLUDES_H
