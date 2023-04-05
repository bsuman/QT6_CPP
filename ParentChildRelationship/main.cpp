
#include <QCoreApplication>
#include <QTimer>
#include "ptest.h"
#include <Qstring>
// display parent child relationship

Ptest * getTestObject(QObject * parent){
    return new Ptest(parent);
}

typedef QList<Ptest *> testlist;

testlist getList(){
    testlist plist;
    for(int i = 0;i<10;i++){
        plist.append(new Ptest());
        plist.last()->setObjectName("PtestObject"+QString::number(i));
    }
    return plist;
}

void display(testlist plist)
{
    foreach(Ptest *item,plist){
        if(item!=nullptr)
            qInfo()<<item;
    }
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    // the Ptest class inheriting from the QObject, will die when the parent class object goes out of scope.
    // code to illustrate parent child relationship in terms of life cycle of the object
    Ptest *p = getTestObject(&a);
    QTimer timer;
    // single shot timer to indicate when the Application should shutdown
    // single shot timer fires only once

    testlist tmp = getList();
    display(tmp); // casuses memory leak as the new objects are never deleted
    // to fix this issue, use qdeleteAll() to remove every object of the QList container.
    //1.way
    //qDeleteAll(tmp);

    // 2.way
    qDeleteAll(tmp.begin(),tmp.end());
    tmp.clear();

    timer.singleShot(2000,&a, QCoreApplication::quit);
    int val = a.exec();
    qInfo()<<"Application exiting with code:" << val;
    return val;
}
