#include "client_controller.h"
#include "client_link.h"
#include "rpc_global.h"
#include "client_rpc_protocol.h"
#include "handler.h"

ClientController::ClientController(QObject* parent, MetaService* service)
    : Controller(parent, service) {
#ifdef DEBUG_RPCCLIENT
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
    m_finished = false;
    m_pollingTimer = new QTimer(this);
    ClientRpcProtocolFactory::instance()->createProtocol(CC_CAST(this));
    connect(m_pollingTimer,SIGNAL(timeout()),this,SLOT(checkDone()));
    m_pollingCounter = 0;
}

ClientController::~ClientController() {}

void ClientController::process(Packet& packet) {
#ifdef DEBUG_RPCCLIENT
    qDebug() << "ClientController::process: " << m_msgLookup.value(packet.getMsg());
#endif
    m_protocol->handlerForHandlerType(packet.getMsg())->handle(packet);
}

bool ClientController::waitFor(Packet& packet) {

    m_lastPacket = packet;
    while(!m_finished) {
        if (m_pollingCounter > POLL_TIMEOUT_COUNT) {
            qWarning() << "Timed out waiting for server response.";
            return false;
        }

        qApp->processEvents();
    }

    return m_finished;
}

void ClientController::checkDone(){

    m_pollingCounter++;
}

void ClientController::confirm(Packet & packet) {

    m_link->dispatch(packet);
}
void ClientController::setProtocol(Protocol * protocol) {
    m_protocol = protocol;
}
Protocol* ClientController::protocol() {
    return m_protocol;
}

Handler* ClientController::response(Handler * handler) {

    Q_UNUSED(handler);
    return 0;
}
void ClientController::setState(ControllerState state) {

    Controller::setState(state);
}
Controller::ControllerState ClientController::state() const {

    return Controller::state();
}
void ClientController::createLink() {

    ClientLink* link = new ClientLink(this);

    m_link = link;
    if (link->setup(0)) {
        if (link->isUp()) {
            setState(READY);
        }
    } else {
        qWarning("Unable to establish link with remote host.");
    }
}

void ClientController::setWaiting(bool waiting) {

    m_finished = !waiting;
    if (waiting) {
        setState(BUSY);
        m_pollingTimer->start(POLL_WAIT_INTERVAL);
    } else {
        m_pollingTimer->stop();
    }
}

Link* ClientController::link() const {

    return m_link;
}
