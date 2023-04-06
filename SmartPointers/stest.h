
#ifndef STEST_H
#define STEST_H
#include <QString>
#include <QDebug>
#include <QObject>


class sTest : public QObject
{
    Q_OBJECT
public:
    explicit sTest(QObject *parent = nullptr);
    ~sTest();
    void print_message(QString str);

signals:

};

#endif // STEST_H
