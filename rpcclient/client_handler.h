#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H
#include "handler.h"



class ClientLinkRequestHandler : public RequestHandler {

     Q_OBJECT

public:
    ClientLinkRequestHandler(Controller* controller);
    virtual ~ClientLinkRequestHandler();

signals:
    void requestLink(qintptr);

protected:
    virtual void handle(Packet& packet);
};

class ClientDisconnectRequestHandler : public RequestHandler {

public:
    ClientDisconnectRequestHandler(Controller* controller);
    virtual ~ClientDisconnectRequestHandler();

protected:
    virtual void handle(Packet&);
};


class ClientSignalForwardRequestHandler : public RequestHandler {

public:
    ClientSignalForwardRequestHandler(Controller* controller);
    virtual ~ClientSignalForwardRequestHandler();

protected:
    virtual void handle(Packet &);
};

class ClientSignalConnectionRequestHandler : public ClientSignalForwardRequestHandler {

public:
    ClientSignalConnectionRequestHandler(Controller* controller);
    virtual ~ClientSignalConnectionRequestHandler();

protected:
    virtual void handle(Packet&);
};

class methodData_t;
class ClientMethodCallRequestHandler : public RequestHandler {

public:
    ClientMethodCallRequestHandler(Controller* controller);
    virtual ~ClientMethodCallRequestHandler();

protected:
    virtual void handle(Packet &);


};

class ClientLinkConfirmationHandler : public ConfirmationHandler {

public:
    ClientLinkConfirmationHandler(Controller* controller);
    virtual ~ClientLinkConfirmationHandler();
protected:
    void handle(Packet &);
};

class ClientMethodCallConfirmationHandler : public ConfirmationHandler {

public:
    ClientMethodCallConfirmationHandler(Controller* controller);
    virtual ~ClientMethodCallConfirmationHandler();
protected:
    void handle(Packet &);
};

class ClientSignalForwardConfirmationHandler : public ConfirmationHandler {

public:
    ClientSignalForwardConfirmationHandler(Controller* controller);
    virtual ~ClientSignalForwardConfirmationHandler();
protected:
    virtual void handle(Packet &);
};

class ClientSignalConnectionConfirmationHandler : public ClientSignalForwardConfirmationHandler {

public:
    ClientSignalConnectionConfirmationHandler(Controller* controller);
    ~ClientSignalConnectionConfirmationHandler();
protected:
    virtual void handle(Packet &);
};

class ClientDisconnectConfirmationHandler : public ConfirmationHandler {

public:
    ClientDisconnectConfirmationHandler(Controller* controller);
    ~ClientDisconnectConfirmationHandler();
protected:
    virtual void handle(Packet &);
};

class ClientHandlerFactory : public AbstractHandlerFactory {

    ClientHandlerFactory();

public:
    ~ClientHandlerFactory();
    static AbstractHandlerFactory* instance() {

        if (!s_instance) {
            s_instance = new ClientHandlerFactory();
        }
        return s_instance;
    }
    virtual bool addPair(HandlerType request, HandlerType response, Controller* controller);

private:
    static AbstractHandlerFactory* s_instance;
    Handler* createRequest(HandlerType request, Controller* controller);
    Handler* createResponse(HandlerType response, Controller* controller);
};
#endif // CLIENT_HANDLER_H


