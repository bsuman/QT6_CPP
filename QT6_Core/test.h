
#ifndef TEST_H
#define TEST_H


#include <QObject>


class test : public QObject
{
    // input to the build process through metaobject compiler to make the class a QObjectType
    Q_OBJECT
public:
    // optional parent, parent child relationship
    // allows automatic memory managment
    explicit test(QObject *parent = nullptr);

signals:
    // communication channels between Qobjects
    // signal is connected to slots of one or more objects
    // when sender fires the signal,
    // the objects whose slots are connected to the signal will see the event.
    // allows communication between threads

    void close(); // dont need to implement the signal as the implementation is done by the MOC system

public slots:
    void dostuff();

};

#endif // TEST_H
