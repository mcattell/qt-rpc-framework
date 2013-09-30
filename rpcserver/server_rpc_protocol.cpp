#include "server_rpc_protocol.h"
#include "server_handler.h"

AbstractProtocolFactory* ServerRpcProtocolFactory::s_instance = 0;


Handler* ServerRpcProtocol::map(Handler* handler) {

    return m_rpcProtocol.value(handler);
}
void ServerRpcProtocol::addMapping(Handler* request, Handler* confirmation) {

    m_rpcProtocol.insert(request,confirmation);
}
HandlerType ServerRpcProtocol::handlerTypeForHandler(Handler* handler) const {

    return m_mapping.value(handler);
}
Handler* ServerRpcProtocol::handlerForHandlerType(HandlerType type) const {

    return m_reverseMapping.value(type);
}
void ServerRpcProtocol::setHandlerTypeTohandlerMapping(HandlerType type, Handler* handler) {

    m_mapping.insert(handler,type);
    m_reverseMapping.insert(type,handler);
}

AbstractProtocolFactory* ServerRpcProtocolFactory::instance() {

    if (s_instance == 0) {

        s_instance = new ServerRpcProtocolFactory();
    }
    return s_instance;
}
void ServerRpcProtocolFactory::createProtocol(Controller* controller) {

    controller->setProtocol(new ServerRpcProtocol());


    AbstractHandlerFactory* factory = ServerHandlerFactory::instance();

    for (int rows = 0; rows < PROTOCOL_PAIRS; ++rows) {

        if (!factory->addPair(table[rows][REQ],table[rows][CONF],controller)) {

            qWarning() << "RpcProtocolFactory::createProtocol(Controller*) failed to add protocol pair.";
            controller->setState(Controller::ERROR);
        }
    }
    controller->setState(Controller::READY);
}
