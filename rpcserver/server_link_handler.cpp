#include "server_handler.h"
#include "server_controller.h"
#include "packet.h"
#include <QtCore/QDebug>
#include "rpcserver_p.h"
#include "server_rpc_protocol.h"
#include "server_metaservice.h"
#include "server_packetfactory.h"

ServerLinkRequestHandler::ServerLinkRequestHandler(Controller *controller)
    : RequestHandler(controller) {
}
ServerLinkRequestHandler::~ServerLinkRequestHandler(){}

ServerLinkConfirmationHandler::ServerLinkConfirmationHandler(Controller *controller)
    : ConfirmationHandler(controller) {
}
ServerLinkConfirmationHandler::~ServerLinkConfirmationHandler(){}

ServerDisconnectRequestHandler::ServerDisconnectRequestHandler(Controller *controller)
    : RequestHandler(controller) {
}
ServerDisconnectRequestHandler::~ServerDisconnectRequestHandler(){}

ServerDisconnectConfirmationHandler::ServerDisconnectConfirmationHandler(Controller *controller)
    : ConfirmationHandler(controller) {
}
ServerDisconnectConfirmationHandler::~ServerDisconnectConfirmationHandler() {}


void ServerLinkConfirmationHandler::handle(Packet & packet) {

    ServerController* controller = CAST_SC(m_controller);
    packet.setMsg(controller->protocol()->handlerTypeForHandler(this));
    ServerPacketFactory::instance()->create(_CONFIRM_LINK_, controller->metaService(), packet);
    m_controller->confirm(packet);
}

void ServerLinkRequestHandler::handle(Packet & packet) {

    BEGIN_DESERIALIZER
    data = packet.getPayload();
    BindingType type;
    QByteArray className;
    stream >> className >> type;
    END_DESERIALIZER
    if (CAST_SC(m_controller)->metaService()->className() == className) {
        if (CAST_SC(m_controller)->metaService()->bind(type)) {
            Handler* respond = m_controller->response(this);
            respond->handle(packet);
        }
    }
}

void ServerDisconnectRequestHandler::handle(Packet & packet) {
    Q_UNUSED(packet);
}
void ServerDisconnectConfirmationHandler::handle(Packet & packet) {
    Q_UNUSED(packet);
}
