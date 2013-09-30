#include "client_handler.h"
#include "callinterface.h"
#include "packet.h"
#include "client_controller.h"
#include "client_metaservice.h"

ClientMethodCallRequestHandler::ClientMethodCallRequestHandler(Controller *controller)
    : RequestHandler(controller) {

    CallInterfaceFactory::instance()->create(controller);
}
ClientMethodCallRequestHandler::~ClientMethodCallRequestHandler() {
}
ClientMethodCallConfirmationHandler::ClientMethodCallConfirmationHandler(Controller *controller)
    : ConfirmationHandler(controller) {
}
ClientMethodCallConfirmationHandler::~ClientMethodCallConfirmationHandler() {
}
void ClientMethodCallRequestHandler::handle(Packet & packet) {

    m_controller->confirm(packet);
    static_cast<ClientController*>(m_controller)->setWaiting(true);

}
void ClientMethodCallConfirmationHandler::handle(Packet & packet) {

    methodData_t* method = reinterpret_cast<methodData_t*>(static_cast<ClientController*>(m_controller)->m_lastPacket.getUserData());
    BEGIN_DESERIALIZER
            if (method->returnType != "void")
                stream >> method->returnData;
    END_DESERIALIZER
    static_cast<ClientController*>(m_controller)->setWaiting(false);
}

