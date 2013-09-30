#include <QtCore/QDebug>
#include "client_handler.h"
#include "client_controller.h"
#include "client_metaservice.h"


ClientLinkRequestHandler::ClientLinkRequestHandler(Controller *controller)
    : RequestHandler(controller) {
}

ClientLinkRequestHandler::~ClientLinkRequestHandler() {
}

ClientLinkConfirmationHandler::ClientLinkConfirmationHandler(Controller *controller)
    : ConfirmationHandler(controller) {
}

ClientLinkConfirmationHandler::~ClientLinkConfirmationHandler() {
}
ClientDisconnectRequestHandler::ClientDisconnectRequestHandler(Controller *controller)
    : RequestHandler(controller) {
}
ClientDisconnectRequestHandler::~ClientDisconnectRequestHandler(){
}


ClientDisconnectConfirmationHandler::ClientDisconnectConfirmationHandler(Controller *controller)
    : ConfirmationHandler(controller) {
}
ClientDisconnectConfirmationHandler::~ClientDisconnectConfirmationHandler() {
}



void ClientLinkRequestHandler::handle(Packet & packet) {


    CAST_CC(m_controller)->setState(Controller::LINKING);
    emit requestLink(0);
    if (m_controller->state() == Controller::READY) {

        packet.setMsg(CAST_CC(m_controller)->protocol()->handlerTypeForHandler(this));
        packet.setType(_REQUEST_);
        m_controller->confirm(packet);
        CAST_CC(m_controller)->setWaiting(true);
    }
}
void ClientLinkConfirmationHandler::handle(Packet & packet) {

    MetaService* metaService = m_controller->metaService();
    QUuid id;

    BEGIN_DESERIALIZER
    data = packet.getPayload();
    stream >> id >> *metaService;
    END_DESERIALIZER

    m_controller->metaService()->setUuid(id);
    m_controller->setState(Controller::LINKED);
    CAST_CC(m_controller)->setWaiting(false);

}
void ClientDisconnectRequestHandler::handle(Packet & packet) {
    Q_UNUSED(packet);
}

void ClientDisconnectConfirmationHandler::handle(Packet & packet) {
    Q_UNUSED(packet);
}
