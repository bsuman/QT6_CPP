
#include <QCoreApplication>
#include<QString>
#include<QStringList>

/*
 * program to illustrate:
 * the use of Qstring with some of it's member functions.
 * the use of Qstringlist
*/

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
    return a.exec();
}
