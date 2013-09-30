#include "roctestclass.h"

RocTestClass::RocTestClass(QObject *parent) :
    QObject(parent){
}
RocTestClass::~RocTestClass() {
}

void RocTestClass::testMethod1(int num0, bool param1) {
    qDebug() << num0 << param1;
}

int RocTestClass::testMethod2(const QString& msg) {
    static int count = 0;
    int ret = 0;
    ret = count;
    ++count;
    if (count == 1000)
        count = 0;
    return ret;
}

bool RocTestClass::testMethod3(float num) const {

    if (num == 0.5) {
        return true;
    } else {
        return false;
    }

    return false;
}
void RocTestClass::someSlot() {

}
