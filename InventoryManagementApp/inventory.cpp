
#include "inventory.h"

Inventory::Inventory(QObject *parent)
    : QObject{parent}
{
    load();
}

Inventory::~Inventory()
{
    m_items.clear();
}

void Inventory::add(QString name, int quant)
{
    if(m_items.contains(name)){
        m_items[name] = m_items[name] + quant;
    }
    else{
        m_items[name] = quant;
    }
}

void Inventory::remove(QString name, int quant)
{
    if(m_items.contains(name)){
        m_items[name] = m_items[name] - quant;
        if(m_items[name] < 0){
            m_items.remove(name);
            qInfo() <<"Item removed!" ;
        }
    }
    else{
        qWarning() <<"Item not found!" ;
    }

}

void Inventory::list()
{
    qInfo() <<"Number of items!" << m_items.size();
    foreach(QString key,m_items.keys()){
        qInfo() <<"Item:" << key <<"Quantity:" << m_items[key];
    }
}

void Inventory::save()
{
    QFile qf("Inventory.txt");
    if(!qf.open(QIODevice::WriteOnly)){
        qCritical()<<"Could not open the file!!" ;
        return;
    }
    // open the file and get the information
    QDataStream stream(&qf);
    // !!!! set version very critical function to call.
    // handles the encoding under the hood.
    // if the file which is laoded has different encoding, the data might look different
    // to ensure correctness always set the version explicitly
    stream.setVersion(QDataStream::Qt_6_5);

    int num = m_items.size();
    // handles the write/IO
    stream << num;
    foreach(QString key,m_items.keys()){
        qInfo() <<"Saving Item:" << key <<"Quantity:" << m_items[key];
        stream << key;
        stream << m_items[key];
    }
    qf.close();
    qInfo()<<"Saved to database!!";

}

void Inventory::load()
{

    QFile qf("Inventory.txt");
    if(!qf.exists()){
        qWarning()<<"Database file does not exist!!";
        return;
    }
    if(!qf.open(QIODevice::ReadOnly)){
        qCritical()<<"Could not open the file!!" ;
        return;
    }
    // open the file and get the information
    QDataStream stream(&qf);
    // !!!! set version very critical function to call.
    // handles the encoding under the hood.
    // if the file which is laoded has different encoding, the data might look different
    // to ensure correctness always set the version explicitly
    if(stream.version()!= QDataStream::Qt_6_5){
        qCritical()<<"Wrong datastream version!!" ;
        qf.close();
        return;
    }

    m_items.clear();
    int max;
    stream >> max;
    qInfo()<<"Number of items to load:" << max;
    // handles the write/IO
    int value;
    QString  key;
    for(int num = 0; num< max; num ++)
    {
        stream >> key;
        stream >> value;
        m_items.insert(key,value);
    }
    qf.close();
    qInfo()<<"Loaded from database!!";
}

