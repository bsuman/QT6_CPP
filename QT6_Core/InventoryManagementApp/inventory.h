
#ifndef INVENTORY_H
#define INVENTORY_H


#include<QObject>
#include<QMap>
#include<QDebug>
#include<QFile>
#include<QDataStream>

class Inventory : public QObject
{
    Q_OBJECT
    QMap<QString, int> m_items;
public:
    explicit Inventory(QObject *parent = nullptr);
    ~Inventory();
    void add(QString name, int quant);
    void remove(QString name, int quant);
    void list();

signals:

public slots:
    void save();
    void load();

};

#endif // INVENTORY_H
