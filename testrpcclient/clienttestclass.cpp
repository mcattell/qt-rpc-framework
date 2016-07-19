#include "clienttestclass.h"
#include <QtCore/QDebug>
#include <QtCore/QTimer>

ClientTestClass::ClientTestClass(QObject *parent) :
    QObject(parent)
{
    m_testTimer = new QTimer(this);
    connect(m_testTimer,SIGNAL(timeout()),this,SLOT(timerExpired()));
    m_testTimer->start(5);

}
void ClientTestClass::testSlot1(int param) {

    qDebug() << "ClientTestClass::testSlot1(int): invoked with int param: " << param;
}

void ClientTestClass::testSlot2(bool param) {
    qDebug() << "ClientTestClass::testSlot2(bool): invoked with bool set to: " << param;
}
void ClientTestClass::timerExpired() {

    static int count = 0;
    static bool flag = false;

    emit testSignal1(count,flag);

    ++count;
    flag = !flag;
}
void ClientTestClass::debugSlot(int p1, bool p2) {

    qDebug() << p1 << p2;
}
void ClientTestClass::debugSlot2(QString str) {

    qDebug() << str;
}
void ClientTestClass::debugSlot3(QVariant v) {

}
void ClientTestClass::debugSlot4(QVariant s) {

}

void ClientTestClass::testSlot3() {

}
void ClientTestClass::anotherSlot1(int a, int b) {
    qDebug() << a << b;
}
void ClientTestClass::debugSlot2(int, bool) {

    qDebug() << "debugSlot2";
}
void ClientTestClass::testSlot4(bool a, float b) {

    qDebug() << a << b;
}
