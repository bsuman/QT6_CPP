
#include <QCoreApplication>
#include<QScopedPointer>
#include<QSharedPointer>
#include "stest.h"
/*
The goal of the program is demonstrate the use of scoped pointer vs shared pointer.
scoped pointer:
The object is created on the heap and destroyed when the object goes
out of scope using the scoped pointer. The behaviour of the object on the heap is much like
an objected created on the stack.

shared pointer:
The object is created on the heap and is destroyed only when the object is not used anymor.
The detection of the use of the object is done via keeping a reference count.
*/
void checkScopedPointer(){
    QScopedPointer<sTest> sp_sTest(new sTest());
    sp_sTest->print_message("checkScopedPointer function called!");
}

void finish(QSharedPointer<sTest> sp_ptr){
    sp_ptr->print_message("Finish function called");
}

void work(QSharedPointer<sTest> sp_ptr){
    sp_ptr->print_message("Work function called");
}

void step(QSharedPointer<sTest> sp_ptr){
    sp_ptr->print_message("Step function called");
    for(int i =0;i<3;i++){
        work(sp_ptr);
    }
    finish(sp_ptr);
}

void testSharedPointer(sTest *ptr){
    QSharedPointer<sTest> sp_ptr(ptr);
    sp_ptr->print_message("Shared Pointer Created");
    step(sp_ptr);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qInfo()<<"========================";
    checkScopedPointer();
    qInfo()<<"========================";

    qInfo()<<"========================";

    testSharedPointer(new sTest());

    qInfo()<<"========================";
    return a.exec();
}
