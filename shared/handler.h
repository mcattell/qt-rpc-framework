#ifndef HANDLER_H
#define HANDLER_H
#include <QtCore/QByteArray>
#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QHash>

typedef int HandlerType;

class Controller;
class Packet;
class Handler : public QObject
{

public:
    Handler(Controller* controller)
        : QObject()
        , m_controller(controller){}
    ~Handler(){}
    virtual void handle(Packet& packet) = 0;

protected:
    Controller* m_controller;
};

class RequestHandler : public Handler {

public:
    RequestHandler(Controller* controller)
        : Handler(controller) {}
    virtual ~RequestHandler(){}

};

class ConfirmationHandler : public Handler {

public:
    ConfirmationHandler(Controller* controller)
        : Handler(controller) {}
    virtual ~ConfirmationHandler(){}
};

class Protocol;
class AbstractHandlerFactory {

public:
    virtual bool addPair(HandlerType request, HandlerType response, Controller* controller) = 0;

protected:
    virtual Handler* createRequest(HandlerType request, Controller* controller) = 0;
    virtual Handler* createResponse(HandlerType response, Controller* controller) = 0;
};

#endif // HANDLER_H
