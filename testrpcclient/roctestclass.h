#ifndef ROCTESTCLASS_H
#define ROCTESTCLASS_H

#include <QtCore/QObject>
#include "callinterface.h"
class RocTestClass : public QObject
{
    Q_OBJECT

public:
    RocTestClass(QObject *parent = 0);
    ~RocTestClass();

    Q_INVOKABLE void testMethod1(int,bool);
    Q_INVOKABLE int testMethod2(const QString&);
    Q_INVOKABLE bool testMethod3(float) const;

signals:
    void someSignal();

public slots:
    void someSlot();
    
private:
    int m_data;
    bool m_flag;
};

#endif // ROCTESTCLASS_H
