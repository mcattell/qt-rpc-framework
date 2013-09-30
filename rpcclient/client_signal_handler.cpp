#include "client_handler.h"
#include "client_controller.h"

ClientSignalForwardConfirmationHandler::ClientSignalForwardConfirmationHandler(Controller *controller)
    : ConfirmationHandler(controller) {
}
ClientSignalForwardConfirmationHandler::~ClientSignalForwardConfirmationHandler() {
}
ClientSignalConnectionConfirmationHandler::ClientSignalConnectionConfirmationHandler(Controller *controller)
    : ClientSignalForwardConfirmationHandler(controller) {
}
ClientSignalConnectionConfirmationHandler::~ClientSignalConnectionConfirmationHandler() {
}
ClientSignalConnectionRequestHandler::ClientSignalConnectionRequestHandler(Controller *controller)
    : ClientSignalForwardRequestHandler(controller) {
}
ClientSignalConnectionRequestHandler::~ClientSignalConnectionRequestHandler() {
}

ClientSignalForwardRequestHandler::ClientSignalForwardRequestHandler(Controller *controller)
    : RequestHandler(controller) {
}
ClientSignalForwardRequestHandler::~ClientSignalForwardRequestHandler() {
}

void ClientSignalForwardRequestHandler::handle(Packet & packet) {

    m_controller->confirm(packet);
}
void ClientSignalConnectionRequestHandler::handle(Packet & packet) {

    m_controller->confirm(packet);
}
void ClientSignalConnectionConfirmationHandler::handle(Packet & packet) {

    Q_UNUSED(packet);
}
void ClientSignalForwardConfirmationHandler::handle(Packet & packet) {

    Q_UNUSED(packet);
}

