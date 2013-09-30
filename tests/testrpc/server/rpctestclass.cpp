#include <QtCore/QDebug>
#include "rpctestclass.h"


RpcTestClass::RpcTestClass(QObject *parent)
    : QObject(parent) {
}
RpcTestClass::~RpcTestClass() {
}



void RpcTestClass::testRpcMethod1(int param0, bool param1) {

    qDebug() << "RpcTestClass::testRpcMethod1 called with parameters: "
             << "int param0: "
             << param0
             << "bool param1: "
             << param1;

}

int RpcTestClass::getterTest1(int param0) {

    qDebug() << "RpcTestClass::getterTest1 called with parameter: " << "int param0: " << param0;
    return param0;
}

bool RpcTestClass::getterTest2(bool param0) {

    qDebug() << "RpcTestClass::getterTest2 called with paramter: " << "bool param0: " << param0;
    return param0;
}
