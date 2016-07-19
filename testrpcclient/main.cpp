#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QTime>
#include "rpcclient.h"
#include "clienttestclass.h"
#include "testrpcclass.h"
#include "roctestclass.h"
#include "sourcetestobject.h"
#include "sourcetestclasstwo.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//    RocTestClass r;
//    r.setObjectName("TestObject");
//    RpcClient c(&r);
//    c.bind(&r,"192.168.0.196",5656);

//    int count = 0;
//    while(count < 999) {

//        count = r.testMethod2("Hello world.");
//        r.testMethod1(5,true);
//        qDebug() << r.testMethod3(0.5);
//        qDebug() << r.testMethod3(1.6);
//    }

    ClientTestClass ctc(0);
    SourceTestObject st(0);
    SourceTestClassTwo stc2(0);
    RpcClient c2(&ctc);
    QUuid id = c2.bind("ClientTestClass","127.0.0.1",5757);
    c2.connectSignalToRemoteSlot(&st,"testSignal1(int,int)","anotherSlot1(int,int)",id);
    c2.connectSignalToRemoteSlot(&stc2,"testSignal1(int,int)","anotherSlot1(int,int)",id);
    c2.connectSignalToRemoteSlot(&stc2,"testSignal2(bool,float)","testSlot4(bool,float)",id);
    return a.exec();
}
