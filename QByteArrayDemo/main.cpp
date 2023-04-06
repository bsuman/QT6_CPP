
#include <QCoreApplication>
#include <QByteArray>

/*
QByteArray allows dynamic allocation of memory.
QByteArray has encoding builtin.
reserve: allocates the memory
resize: allocates the memory and use the memory
squeeze: free the unsused memory
truncate: chop off the bytearray at the given length
*/

void print_stats(QByteArray &data){
    qInfo()<<"Length of the data:" << data.length();
    qInfo()<<"Capacity of the data:" << data.capacity();
    qInfo() <<"Data:"<<data;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qInfo()<<"========= Constructing ByteArray =========";

    QByteArray t_arr;
    t_arr.append("Hello");
    qInfo()<<t_arr;

    QByteArray n_arr("Hello");
    qInfo()<<n_arr;

    // create QByteArray from QString using toLocal8Bit
    QByteArray n_arr_f_string(QString("QString to QByteArray").toLocal8Bit());
    qInfo()<<n_arr_f_string;

    qInfo()<<"========= Allocation of memory to ByteArray =========";

    QByteArray data;
    print_stats(data);

    data.append("check size()");
    print_stats(data);

    data.reserve(20);
    print_stats(data);

    data.resize(25);
    print_stats(data);

    qInfo()<<"========= Deallocation of memory from ByteArray =========";

    data.squeeze();
    print_stats(data);

    data.truncate(16);
    print_stats(data);

    data.clear();
    print_stats(data);

    qInfo()<<"====== Accessing ByteArray Elements=========";
    data.append("Adding new data");

    for(int var = 0; var < data.length(); var++) {
        qInfo()<<" Character at index"<< var << "is"<< data.at(var);
    }

    foreach(char var,data) {
        qInfo()<<" Character is"<< var;
    }

    qInfo()<<"====== Changing respresentation of ByteArray Elements=========";

    QByteArray hex = data.toHex();
    qInfo() <<"To Hex" << hex;
    QByteArray from_hex=  QByteArray::fromHex(hex);
    qInfo() <<"From Hex" << from_hex;


    return a.exec();
}
