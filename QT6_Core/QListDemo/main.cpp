
#include <QCoreApplication>
#include <QList>
#include <QTextStream>

/*
Qlist templated container for storing data.
Dynamic memory allocation with max allocation size of 2GiB.
Copy of Qlist can be made but could be expensive computation.

QMap templated class to make the associative container mapping a key to a value
Key should be unique
Values dont have to be unique

QTextStream:
Input output using QIODevice Class
Qt::endl equivlaent to doing flush on the stdout

*/


void CheckList(){
    qInfo()<<"==============Creating a list============== ";

    QList<int> int_list = {10,12,43,34,13};
    qInfo()<< "Size of qlist of integers" <<  int_list.length();

    qInfo()<<"==============Accessing a list============== ";
    foreach(auto item,int_list ){
        qInfo()<< "Item of qlist of integers" << item;
    }
    qInfo()<<"==============Modifying a list============== ";
    int_list.push_back(12);
    qInfo()<< "Size of qlist of integers" <<  int_list.length();
    qInfo()<< "Last Item of qlist of integers" <<  int_list.last();
    int_list.push_front(-100);
    qInfo()<< "First Item of qlist of integers" <<  int_list.front();

    qInfo()<<"==============counting occurences in the list============== ";
    qInfo()<< "Count of 12 qlist of integers" <<  int_list.count(12);

    qInfo()<<"==============find first occurence in the list============== ";
    qInfo()<< "Index of 12 qlist of integers" <<  int_list.indexOf(12);
    qInfo()<< "Index of 256 qlist of integers" <<  int_list.indexOf(256);

    qInfo()<<"==============get slice of the list============== ";
    QList<int> slice(int_list.sliced(3,2));
    qInfo()<< "Item of slice of integers" << slice;
}


void CheckMap()
{
    qInfo()<<"==============Creating a QMap============== ";
    QMap<int,QString> keyValueMap;
    qInfo()<<"==============Inserting/Adding key value to a QMap ============== ";
    keyValueMap[1] = "one_string";
    keyValueMap[2] = "two_string";
    qInfo()<<"==============Accessing members of a QMap using iterators============== ";

    for(auto c = keyValueMap.constKeyValueBegin();c!= keyValueMap.constKeyValueEnd();c++){
        qInfo()<<"Key" << c->first << "value" << c->second;
    }

    qInfo()<<"==============Accessing members of a QMap using keys============== ";
    foreach (int c, keyValueMap.keys())
    {
        qInfo()<<"Key" << c << "value" << keyValueMap.value(c) ;
    }
    qInfo()<<"==============Finding members of a QMap using keys============== ";
    int key = 1;
    if(keyValueMap.find(key)!=keyValueMap.end()){
        keyValueMap[key] = "one_new_value";
        qInfo()<<"Key" <<key << "with modified value" << keyValueMap.value(key) ;
    }

}

void CheckQTextStream(){

    QTextStream q_input(stdin);
    QTextStream q_out(stdout);
    q_out<<"Enter your name and age:\n" ;
    q_out.flush();
    QString name;
    name = q_input.readLine();
    double age = q_input.readLine().toDouble();
    q_out<<"Entered name is:"<< name <<" and age is "<<age <<Qt::endl;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    CheckList();
    CheckMap();
    CheckQTextStream();
    return a.exec();
}
