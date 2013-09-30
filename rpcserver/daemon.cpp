#include "daemon.h"

Daemon::Daemon(QObject *parent) :
    QTcpServer(parent)
{
    qRegisterMetaType<qintptr>("qintptr");
}
void Daemon::start() {
    listen(QHostAddress::AnyIPv4,m_port);
}
void Daemon::incomingConnection(qintptr handle) {
    emit notifyConnection(handle);
}
