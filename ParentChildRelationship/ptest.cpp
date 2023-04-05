
#include "ptest.h"

Ptest::Ptest(QObject *parent)
    : QObject{parent}
{
    qInfo()<<this<<"PTest Object Constructed"<<parent;
}


Ptest::~Ptest()
{
    qInfo()<<this<<"PTest Object destroyed"<<parent();
}


