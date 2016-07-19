#include <QtCore/QDebug>
#include <QtCore/QTimer>
#include "server_handler.h"
#include "server_controller.h"


ServerSignalForwardRequestHandler::ServerSignalForwardRequestHandler(Controller *controller)
    : RequestHandler(controller) {

    m_scheduler = new QTimer(this);
    connect(m_scheduler, SIGNAL(timeout()),this,SLOT(dequeueSignal()));
    m_scheduler->start(1);
}

ServerSignalForwardRequestHandler::~ServerSignalForwardRequestHandler() {}

ServerSignalConnectionRequestHandler::ServerSignalConnectionRequestHandler(Controller* controller)
    : ServerSignalForwardRequestHandler(controller){}

ServerSignalConnectionRequestHandler::~ServerSignalConnectionRequestHandler(){}

ServerSignalForwardConfirmationHandler::ServerSignalForwardConfirmationHandler(Controller *controller)
    : ConfirmationHandler(controller) {}

ServerSignalForwardConfirmationHandler::~ServerSignalForwardConfirmationHandler(){}

ServerSignalConnectionConfirmationHandler::ServerSignalConnectionConfirmationHandler(Controller *controller)
    : ServerSignalForwardConfirmationHandler(controller) {}

ServerSignalConnectionConfirmationHandler::~ServerSignalConnectionConfirmationHandler() {}



void ServerSignalForwardRequestHandler::handle(Packet& packet) {

    BEGIN_DESERIALIZER

    data = packet.getPayload();
    SignalCall sig(stream, m_controller->metaService());
    m_signalQueue.enqueue(sig);
    END_DESERIALIZER
}
void ServerSignalForwardRequestHandler::dequeueSignal() {

    if (!m_signalQueue.isEmpty()) {

        SignalCall item(m_signalQueue.head());
        if (item.call())
            m_signalQueue.dequeue();
    }
}

void ServerSignalConnectionRequestHandler::handle(Packet& packet) {

    int signal, slot;
    QUuid key;
    int mode;
    BEGIN_DESERIALIZER

    data = packet.getPayload();
    stream >> key
           >> signal
           >> slot
           >> mode;

    END_DESERIALIZER

    m_controller->metaService()->connectRemoteSignalToSlot(signal, m_controller->metaService()->subject(), slot, (Rpc::BlockingMode)mode);
}
void ServerSignalConnectionConfirmationHandler::handle(Packet & packet) {
    Q_UNUSED(packet);
}
void ServerSignalForwardConfirmationHandler::handle(Packet & packet) {
    Q_UNUSED(packet);
}






