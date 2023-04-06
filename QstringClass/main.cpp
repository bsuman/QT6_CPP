
#include <QCoreApplication>
#include <QString>
#include <QStringList>
#include <QStringView>

/*
 * program to illustrate:
 * the use of Qstring with some of it's member functions.
 *  Qstring is not a QObject.
 *  It is does allow call by value and create copy of the input arguments
 * the use of QStringlist
 * the use of QStringView
 * class to view the string without modifying the Qstring
 * QStringView is faster because: it holds the pointer to the string and
 * does not copy the string when used as argument.
 *
*/


void readname(QString name){
    name.insert(0,"Ms."); // creates a copy of input string and modifies the copied string
    qInfo()<< "Name inside readname function:"<<name;
}

void readonlyname(QStringView name){
    //name.insert(0,"Ms."); // modifying the underlying string is not allowed
    qInfo()<< "Name inside readonlyname function:"<<name.data();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString s("First Qstring!");
    qInfo()<< s;

    qInfo()<< s.contains("irst");
    int val(12), pos(34);

    QString snew = "Find the values %1 and %2";
    qInfo()<< snew.arg(val).arg(pos);

    QString ssec("Second Qstring!");
    qInfo()<<"Comparing the two strings" << ssec.compare(s);

    qInfo()<<"ends with Qstring!" << ssec.endsWith("Qstring!");

    qInfo()<<"Qstring at index:" << ssec.indexOf("Qstring!");

    QStringList ls = ssec.split(" ");
    foreach(QString s,ls){
        qInfo()<< "Split String:"<<s;
    }

    QString name("Suman Bidarahalli");
    readname(name);
    qInfo()<< "Name:"<<name;

    return a.exec();
}
