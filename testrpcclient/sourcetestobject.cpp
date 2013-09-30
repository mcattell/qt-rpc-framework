#include "sourcetestobject.h"
#include <QtCore/QTimer>

SourceTestObject::SourceTestObject(QObject *parent) :
    QObject(parent)
{
    m_timer = new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this, SLOT(timeout()));
    m_timer->start(500);
}
void SourceTestObject::timeout() {

    emit testSignal1(50,60);
}
