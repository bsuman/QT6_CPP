
#ifndef PTEST_H
#define PTEST_H


#include <QObject>
#include <QDebug>

class Ptest : public QObject
{
    Q_OBJECT
public:
    explicit Ptest(QObject *parent = nullptr);
    ~Ptest();
signals:

};

#endif // PTEST_H
