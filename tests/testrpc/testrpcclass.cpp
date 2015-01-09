#include "testrpcclass.h"
#include <QtCore/QDebug>

TestRpcClass::TestRpcClass(QObject *parent) :
    QObject(parent)
{
}
void TestRpcClass::testSlot1(int param0, bool param1) {

    qDebug() << "TestRpcClass::testSlot1: param0 (int) :" << param0 << " param1 (bool): " << param1;

}

void TestRpcClass::testSlot2(QString param0) {
    qDebug() << "testRpcClass::testSlot2: param0 (QString) : " << param0;
}
void TestRpcClass::testMethod(int param) {

}
void TestRpcClass::testSlot3(int param0, bool param1) {
    qDebug() << "testSlot3: " << param0 << param1;
}
 void TestRpcClass::onStatusChanged(Rpc::Status, const QUuid& id) {

     qDebug() << "The rpc client signalled that it is inactive";
 }
