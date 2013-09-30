#ifndef LINK_H
#define LINK_H
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QtCore/QDataStream>
#include "packet.h"

class Controller;
class Link : public QTcpSocket {

    Q_OBJECT

public:
    Link(QObject* parent, Controller* controller);
    virtual ~Link();
    virtual void schedule(Packet&) = 0;
    virtual void dispatch(Packet&) = 0;


public slots:
    virtual bool setup(qintptr) = 0;
private slots:
    virtual void rx();
    void onSocketStateChanged(QAbstractSocket::SocketState);

protected:
    virtual bool isUp() = 0;
    virtual bool tearDown() = 0;
    virtual void tx(Packet& payload);


    Controller* m_controller;
private:
    int m_size;
};

#endif // LINK_H
