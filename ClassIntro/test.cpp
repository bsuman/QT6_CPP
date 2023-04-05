
#include "test.h"
#include<qdebug.h>
test::test(QObject *parent)
    : QObject{parent}
{

}

void test::dostuff()
{
    qInfo()<<"Doing stuff!!";
    // how to emit/fire a signal
    // to make connect the signal and the slot
    emit close();
}

