#include "client_rpc_protocol.h"
#include "client_handler.h"

AbstractProtocolFactory* ClientRpcProtocolFactory::s_instance = 0;


Handler* ClientRpcProtocol::map(Handler* handler) {

    return m_rpcProtocol.value(handler);
}
void ClientRpcProtocol::addMapping(Handler* request, Handler* confirmation) {

    m_rpcProtocol.insert(request,confirmation);
}
HandlerType ClientRpcProtocol::handlerTypeForHandler(Handler* handler) const {

    return m_mapping.value(handler);
}
Handler* ClientRpcProtocol::handlerForHandlerType(HandlerType type) const {

    return m_reverseMapping.value(type);
}
void ClientRpcProtocol::setHandlerTypeTohandlerMapping(HandlerType type, Handler* handler) {

    m_mapping.insert(handler,type);
    m_reverseMapping.insert(type,handler);
}

AbstractProtocolFactory* ClientRpcProtocolFactory::instance() {

    if (s_instance == 0) {

        s_instance = new ClientRpcProtocolFactory();
    }
    return s_instance;
}
void ClientRpcProtocolFactory::createProtocol(Controller* controller) {

    controller->setProtocol(new ClientRpcProtocol());

    AbstractHandlerFactory* factory = ClientHandlerFactory::instance();

    for (int rows = 0; rows < PROTOCOL_PAIRS; ++rows) {

        if (!factory->addPair(table[rows][REQ],table[rows][CONF],controller)) {

            qWarning() << "RpcProtocolFactory::createProtocol(Controller*) failed to add protocol pair.";
            controller->setState(Controller::ERROR);
        }
    }
    controller->setState(Controller::READY);
}
