
#include <QCoreApplication>
#include <QSqlDatabase>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    connectToDatabase();
    return a.exec();
}
