// QT includes
#include <QCoreApplication>
#include<QTextStream>
#include<QDebug>

//CPP includes
#include<iostream>
#include<string>

void cpp_doit(){
    std::string name;
    std::cout<<"Enter your name ";
    std::cin>> name;
    std::cout<<"Entered name is "<< name << "\n";

}

void QT_doit(){
    QTextStream input(stdin);
    QTextStream msg(stdout);
    msg << "Enter you name ";
    msg.flush();
    QString string = input.readLine();
    msg << "Entered name is:" << string;
}

void using_QDebug(){
    qInfo()<<"Enter your name";
    QTextStream input(stdin);
    QString string = input.readLine();
    qInfo()<<"Entered name is:" << string;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    // cpp_doit();
    // QT_doit();
    using_QDebug();
    return a.exec();
}
