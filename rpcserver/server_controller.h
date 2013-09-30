#ifndef SERVERCONTROLLER_H
#define SERVERCONTROLLER_H
#include <QtCore/QString>
#include <QtCore/QMap>
#include "controller.h"
#include "rpcserver_p.h"
#include "handler.h"
#include "packet.h"

#define CAST_SC(c) static_cast<ServerController*>(c)
#define SC_CAST(c) dynamic_cast<Controller*>(c)

class Packet;
class Link;
class Protocol;

class ServerController : public Controller
{
    Q_OBJECT

public:

    ServerController(QObject* parent, MetaService *service);

    virtual ~ServerController();
    virtual void process(Packet&);
    virtual bool waitFor(Packet&);
    virtual void confirm(Packet &);
    virtual void setProtocol(Protocol *);
    virtual Protocol* protocol();
    virtual Handler* response(Handler*);
    virtual Link* link() const;

    Packet& lastPacket() ;


signals:
    void notifyLink(qintptr);

public slots:
    void acceptConnection(qintptr);

private:
    Link* m_link;
    Protocol* m_protocol;
    Packet m_lastPacket;
    QByteArray m_className;
#ifdef DEBUG_RPC_SERVER
    QMap<quint8,QString> m_msgLookup;
#endif
};

#endif // SERVERCONTROLLER_H
