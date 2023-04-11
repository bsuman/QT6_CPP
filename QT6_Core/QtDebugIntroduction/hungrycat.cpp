
#include "hungrycat.h"
#include <QDebug>

HungryCat::HungryCat(QObject *parent)
    : QObject{parent}
{

}
void HungryCat::meow(){
    qInfo()<<"I am a hungry cat!!";
}
