#ifndef CLIENTCONTROLLER_H
#define CLIENTCONTROLLER_H
#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>
#include <QtCore/QString>
#include <QtCore/QMap>
#include "controller.h"
#include "packet.h"
#include "protocol.h"

#define POLL_WAIT_INTERVAL  10
#define POLL_TIMEOUT_COUNT  500
#define CAST_CC(c) static_cast<ClientController*>(c)
#define CC_CAST(c) dynamic_cast<Controller*>(c)


class Link;
class ClientController :  public Controller
{

    Q_OBJECT

public:
    ClientController(QObject* parent, MetaService* service);
    virtual ~ClientController();

    virtual void process(Packet&);
    virtual bool waitFor(Packet&);
    virtual void confirm(Packet &);
    virtual Handler* response(Handler *);
    virtual void setProtocol(Protocol *);
    virtual Protocol* protocol();
    virtual void setState(ControllerState state);
    virtual ControllerState state() const;
    virtual Link* link() const;
    void setWaiting(bool);
    Packet m_lastPacket;

public slots:
    void checkDone();
    void createLink();

private:
    Link* m_link;
    QTimer *m_pollingTimer;
    bool m_finished;
    int m_pollingCounter;
    Protocol* m_protocol;
#ifdef DEBUG_RPCCLIENT
    QMap<quint8,QString> m_msgLookup;
#endif
};


#endif // CLIENTCONTROLLER_H
