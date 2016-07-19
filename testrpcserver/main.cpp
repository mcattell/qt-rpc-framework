#include <QtCore/QCoreApplication>
#include "rpcserver.h"
//#include "roctestclass.h"
#include "clienttestclass.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    RpcServer r(0);
//    RocTestClass t(0);
    ClientTestClass ct(0);
//    t.setObjectName("RocTestObject");
//    r.registerReceiver(&t,5656);

    RpcServer r1(0);
    ct.setObjectName("TestSignalObject1");
    r1.registerReceiver(&ct,5757);

    return a.exec();
}
