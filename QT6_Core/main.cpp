
#include <QCoreApplication>
#include "test.h"
#include<QDebug>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    test t;
    // connect signal and slots
    // connect the sender(object), the signal from the class of the object
    // To
    // receiver object, the slot from the receiver object class
    // default is autoconnection,
    // but since the receiver life is handled somewhere else, the signal is queued
    QObject::connect(&t,&test::close,&a,&QCoreApplication::quit,Qt::QueuedConnection);
    t.dostuff();
    // exec is an event loop
    int val = a.exec();
    qInfo()<<"Exit val" << val;
    return val;
}
