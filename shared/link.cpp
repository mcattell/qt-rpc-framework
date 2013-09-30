#include "link.h"
#include "controller.h"

Link::Link(QObject *parent, Controller *controller)
    : QTcpSocket(parent)
    ,m_controller(controller){

    connect(this,SIGNAL(readyRead()),this,SLOT(rx()));
    connect(this,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(onSocketStateChanged(QAbstractSocket::SocketState)));
}
Link::~Link() {}

void Link::tx(Packet& payload) {

    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    out << payload;
    write(data);
    if (state() == QAbstractSocket::ConnectedState)
        waitForBytesWritten();
    else
        qWarning("Socket is not connected.");
}
void Link::rx() {

    QDataStream in(this);
    m_size = 0;

    if(bytesAvailable() < ((qint64)sizeof(int)))

        return;

    if (m_size == 0)

        in >> m_size;

    if(bytesAvailable() < m_size)

        return;

    Packet incoming;
    incoming.setSize(m_size);
    in >> incoming;

    m_controller->process(incoming);
    m_size = 0;
}
void Link::onSocketStateChanged(QAbstractSocket::SocketState state){

    if (state == QAbstractSocket::UnconnectedState) {
        m_controller->setState(Controller::ERROR);
    }
}
