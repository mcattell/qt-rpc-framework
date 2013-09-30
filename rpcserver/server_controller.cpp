#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>
#include "server_controller.h"
#include "daemon.h"
#include "server_link.h"
#include "rpc_global.h"
#include "server_handler.h"
#include "server_rpc_protocol.h"

ServerController::ServerController(QObject* parent, MetaService* service)
    : Controller(parent, service){

#ifdef DEBUG_RPC_SERVER
    for (int i = 0; i < 10; ++i) {
        m_msgLookup.insert(0xff,"_REQUEST_LINK_"      );
        m_msgLookup.insert(0xfe,"_CONFIRM_LINK_"      );
        m_msgLookup.insert(0xfd,"_CALL_METHOD_"       );
        m_msgLookup.insert(0xfc,"_RETURN_FROM_CALL"   );
        m_msgLookup.insert(0xfb,"_REQUEST_DISCONNECT_");
        m_msgLookup.insert(0xfa,"_CONFIRM_DISCONNECT_");
        m_msgLookup.insert(0xf8,"_SIGNAL_FORWARD"     );
        m_msgLookup.insert(0xf7,"_SIGNAL_CONNECT_"    );
        m_msgLookup.insert(0xf6,"_SIGNAL_CONF_"       );
        m_msgLookup.insert(0xf5,"_SIGNAL_CONF_CONN_"  );
    }
#endif
    m_link = new ServerLink(this);
    connect(this,SIGNAL(notifyLink(qintptr)),m_link,SLOT(setup(qintptr)));
    ServerRpcProtocolFactory::instance()->createProtocol(SC_CAST(this));
}
ServerController::~ServerController(){}

Protocol* ServerController::protocol() {

    return m_protocol;
}

/************************************************************************************
 * Main entry point for incoming requests
 ************************************************************************************/
void ServerController::process(Packet& packet) {

#ifdef DEBUG_RPC_SERVER
    qDebug() << "ServerController::process: " << m_msgLookup.value(packet.getMsg());
#endif
    if (state() == LINKING)
        m_lastPacket = packet;

    m_protocol->handlerForHandlerType(packet.getMsg())->handle(packet);
}
void ServerController::acceptConnection(qintptr handle) {

    setState(LINKING);
    emit notifyLink(handle);
}

bool ServerController::waitFor(Packet & packet) {

    Q_UNUSED(packet)
    return false;
}

void ServerController::confirm(Packet & packet) {

    m_link->dispatch(packet);
}
void ServerController::setProtocol(Protocol * protocol) {

    m_protocol = protocol;
}
Handler* ServerController::response(Handler * handler) {

    return m_protocol->map(handler);
}

Packet& ServerController::lastPacket() {

    return m_lastPacket;
}
Link* ServerController::link() const {

    return m_link;
}

