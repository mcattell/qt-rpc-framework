#ifndef SERVER_RPC_PROTOCOL_H
#define SERVER_RPC_PROTOCOL_H

#include <QtCore/QDebug>
#include "protocol.h"
#include "controller.h"
#include "rpc_global.h"

#define PROTOCOL_PAIRS 5
#define REQ            0
#define CONF           1

const HandlerType table[PROTOCOL_PAIRS][PAIR] = {
    {_REQUEST_LINK_,_CONFIRM_LINK_},
    {_CALL_METHOD_,_RETURN_FROM_CALL_},
    {_REQUEST_DISCONNECT_,_CONFIRM_DISCONNECT_},
    {_SIGNAL_FORWARD_,_SIGNAL_CONF_},
    {_SIGNAL_CONNECT_,_SIGNAL_CONF_CONN_}
};

struct ServerRpcProtocol : public Protocol {

    virtual Handler* map(Handler* handler);
    virtual void addMapping(Handler* request, Handler* confirmation) ;
    virtual HandlerType handlerTypeForHandler(Handler* handler) const ;
    virtual Handler* handlerForHandlerType(HandlerType type) const ;
    virtual void setHandlerTypeTohandlerMapping(HandlerType type, Handler* handler);

private:
    QHash<Handler*,Handler*> m_rpcProtocol;
    QHash<Handler*,HandlerType> m_mapping;
    QHash<HandlerType,Handler*> m_reverseMapping;
};


class ServerRpcProtocolFactory : AbstractProtocolFactory {

    ServerRpcProtocolFactory(){}
    static AbstractProtocolFactory* s_instance;
    virtual void createProtocol(Controller* controller);

public:
    static AbstractProtocolFactory* instance();
    ~ServerRpcProtocolFactory(){}
};

#endif // SERVER_RPC_PROTOCOL_H
