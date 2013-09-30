#ifndef SERVERLINK_H
#define SERVERLINK_H
#include <QtCore/QQueue>
#include "link.h"
#include "packet.h"

class ServerController;
class QTimer;
class ServerLink :  public Link
{

    Q_OBJECT

public:
    ServerLink(QObject* parent);
    virtual ~ServerLink();

    virtual bool tearDown();
    virtual bool isUp();
    virtual void schedule(Packet &);
    virtual void dispatch(Packet&);

public slots:
    virtual bool setup(qintptr);
    void on_ServerLink_ScheduleTimer();

private:
    void handOff(int,int,QDataStream&);
    QQueue<Packet> m_scheduled;
    QTimer *m_scheduler;
};

#endif // SERVERLINK_H
