#include <QtCore/QDataStream>
#include <QtCore/QTimer>
#include "server_link.h"
#include "server_controller.h"
#include "packet.h"

ServerLink::ServerLink(QObject* parent)
    : Link(parent, static_cast<Controller*>(parent))
{

    m_scheduler = new QTimer(this);
    connect(m_scheduler,SIGNAL(timeout()),this,SLOT(on_ServerLink_ScheduleTimer()));
}
ServerLink::~ServerLink(){}

bool ServerLink::setup(qintptr handle) {

    return setSocketDescriptor(handle);
}

bool ServerLink::tearDown() {

    return false;
}

bool ServerLink::isUp() {

    return false;
}


void ServerLink::on_ServerLink_ScheduleTimer() {

    if (!m_scheduled.isEmpty()) {

        Packet p = m_scheduled.dequeue();
        dispatch(p);
    }
}

void ServerLink::schedule(Packet & packet) {

    Packet in = packet;
    m_scheduled.enqueue(in);
}

void ServerLink::dispatch(Packet& packet) {

    Link::tx(packet);
}

