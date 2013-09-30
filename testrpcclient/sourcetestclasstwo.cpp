#include "sourcetestclasstwo.h"
#include <QtCore/QTimer>

SourceTestClassTwo::SourceTestClassTwo(QObject *parent) :
    QObject(parent)
{
    m_timer = new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this, SLOT(timeout()));
    m_timer->start(800);
}
void SourceTestClassTwo::timeout() {

    static bool flag = true;
    emit testSignal1(10,20);
    emit testSignal2(flag,10.57);
    flag = !flag;
}
