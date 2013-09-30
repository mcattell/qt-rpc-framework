#include <QtCore/QObject>
#include "client_handler.h"
#include "rpc_global.h"
#include "client_controller.h"
#include "client_link.h"

AbstractHandlerFactory* ClientHandlerFactory::s_instance = 0;

ClientHandlerFactory::ClientHandlerFactory(){
}
ClientHandlerFactory::~ClientHandlerFactory(){
}

bool ClientHandlerFactory::addPair(HandlerType request, HandlerType response, Controller* controller) {

    Handler* req = createRequest(request,controller);
    Handler* resp = createResponse(response,controller);

    if (req && resp) {
        controller->protocol()->addMapping(req,resp);
        controller->protocol()->setHandlerTypeTohandlerMapping(request,req);
        controller->protocol()->setHandlerTypeTohandlerMapping(response,resp);
        return true;
    }
    return false;
}


Handler* ClientHandlerFactory::createRequest(HandlerType type, Controller* controller) {

    Handler* handler = 0;
    switch (type) {
    case _REQUEST_LINK_: {
        handler = new ClientLinkRequestHandler(controller);
        QObject::connect(static_cast<ClientLinkRequestHandler*>(handler)
                         ,SIGNAL(requestLink(qintptr))
                         ,CAST_CC(controller)
                         ,SLOT(createLink()));

    }break;
    case _CALL_METHOD_: {
        handler = new ClientMethodCallRequestHandler(controller);
    }break;
    case _REQUEST_DISCONNECT_: {
        handler = new ClientDisconnectRequestHandler(controller);
    }break;
    case _SIGNAL_FORWARD_: {
        handler = new ClientSignalForwardRequestHandler(controller);
    }break;
    case _SIGNAL_CONNECT_: {
        handler = new ClientSignalConnectionRequestHandler(controller);
    }break;

    }
    return handler;
}
Handler* ClientHandlerFactory::createResponse(HandlerType type, Controller* controller) {

    Handler* handler = 0;
    switch (type) {
    case _CONFIRM_LINK_: {
        handler = new ClientLinkConfirmationHandler(controller);
    }break;
    case _RETURN_FROM_CALL_: {
        handler = new ClientMethodCallConfirmationHandler(controller);
    }break;
    case _CONFIRM_DISCONNECT_: {
        handler = new ClientDisconnectConfirmationHandler(controller);
    }break;
    case _SIGNAL_CONF_: {
        handler = new ClientSignalForwardConfirmationHandler(controller);
    }break;
    case _SIGNAL_CONF_CONN_: {
        handler = new ClientSignalConnectionConfirmationHandler(controller);
    }break;
    }
    return handler;
}

