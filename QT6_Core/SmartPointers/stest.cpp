
#include "stest.h"

sTest::sTest(QObject *parent)
    : QObject{parent}
{
    qInfo()<< "Constructor of the sTest Class called !!";
}

sTest::~sTest(){
    qInfo()<< "Destructor of the sTest Class called !!";
}

void sTest::print_message(QString message)
{
    qInfo()<<message;
}
