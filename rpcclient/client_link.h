#ifndef CLIENTLINK_H
#define CLIENTLINK_H
#include "../shared/link.h"
#include <QtCore/QUuid>
#include <QtNetwork/QHostAddress>

class Controller;
class ClientLink : public Link
{

    Q_OBJECT

public:
    ClientLink(Controller* controller);
    virtual ~ClientLink();
    virtual bool isUp();
    virtual bool tearDown();
    virtual void schedule(Packet &);
    virtual void dispatch(Packet&);
    inline void setHostAddress(const QHostAddress& address) {
        m_remote = address;
    }
    inline virtual void setPort(qint16 port) {
        m_port = port;
    }
    inline qint16 port() const {
        return m_port;
    }

public slots:
    bool setup(qintptr);

private:
    QHostAddress m_remote;
    qint16 m_port;
};

#endif // CLIENTLINK_H
