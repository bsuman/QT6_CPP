
#include <QCoreApplication>
#include <QTextStream>
#include "inventory.h"
/*
* Console Application for managing an inventory.
* QMap is used to load the "information" from the database.
* The Database is implemented using the QFile.
* The application provides the interface for the user to perform actions on the inventory.
* QDataStream class is used to read and write to the IODevice like a file.
* // QDataStream used for serialization i.e. conversion of the input data into machine readable code/language.
* Goal: apply all the QT knowledge gained until now..
*/

// Command to the user:
// add items, quantity
// remove items, quantity
// get inventory list


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTextStream qIn(stdin);
    Inventory iv;
    qInfo() << "**** Welcome to the iventory managament application ****";
    qInfo() << "Available Commands";
    qInfo() << "add <item_name> <item_quantity>";
    qInfo() << "remove <item_name> <item_quantity>";
    qInfo() << "save";
    qInfo() << "load";
    qInfo() << "list";
    qInfo() << "quit";
    QString input;
    while(true){
        qInfo() << "Enter a command";
        input= qIn.readLine();
        QStringList iInputlist =  input.split(" ");
        if(iInputlist.size() <0){
            break;
        }
        input = iInputlist.at(0).toUpper();
        if(input == "QUIT"){
            break;
        }
        if(input == "ADD" || input == "REMOVE"){
            if(iInputlist.size() < 3){
                qInfo() << "Not enough info!!";
                continue;
            }
            QString name = iInputlist.at(1);
            bool isInt;
            int qty= iInputlist.at(2).toInt(&isInt);
            if(!isInt){
                qInfo() << "Invalid quantity!!";
                continue;
            }
            if(input == "ADD") iv.add(name,qty);
            if(input == "REMOVE") iv.remove(name,qty);
        }
        if(input == "LOAD"){
            iv.load();
        }
        if(input == "LIST"){
            iv.list();
        }
        if(input == "SAVE"){
            iv.save();
        }
    }
    iv.save();
    qInfo() << "Complete!!";
    return a.exec();
}
