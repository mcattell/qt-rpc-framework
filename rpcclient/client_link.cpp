#include <QtCore/QSettings>
#include "client_link.h"
#include "packet.h"
#include "client_controller.h"
#include "client_metaservice.h"

ClientLink::ClientLink(Controller* controller)
    : Link(0, controller){  
}
ClientLink::~ClientLink(){}


bool ClientLink::setup(qintptr) {

    m_port = m_controller->metaService()->port();
    m_remote = static_cast<ClientMetaService*>(m_controller->metaService())->hostAddress();
    connectToHost(m_remote, m_port);
    return waitForConnected(5000);
}

bool ClientLink::isUp() {

    return (QAbstractSocket::state() == QAbstractSocket::ConnectedState);
}
bool ClientLink::tearDown() {

    return false;
}
void ClientLink::schedule(Packet &){}
void ClientLink::dispatch(Packet& packet) {

    Link::tx(packet);
}
