#ifndef RPCTESTCLASS_H
#define RPCTESTCLASS_H
#include <QtCore/QObject>
#include "callinterface.h"

class RpcTestClass : public QObject
{
    Q_OBJECT
public:
    RpcTestClass(QObject* parent = 0);
    virtual ~RpcTestClass();
    Q_INVOKABLE void testRpcMethod1(int, bool);
    Q_INVOKABLE int getterTest1(int);
    Q_INVOKABLE bool getterTest2(bool);
    
};


#endif // RPCTESTCLASS_H
