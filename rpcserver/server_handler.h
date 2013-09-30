#ifndef SERVER_HANDLER_H
#define SERVER_HANDLER_H
#include <QtCore/QDataStream>
#include <QtCore/QQueue>
#include <QtCore/QVector>
#include <QtCore/QUuid>
#include "handler.h"
#include "server_metaservice.h"


struct SignalCall {

    SignalCall(){}
    SignalCall(QDataStream& stream, MetaService* service)
        : m_service(service) {

        m_args.append(0);
        stream >> m_key
               >> m_signal
               >> m_slot
               >> m_paramCount;

        for (int i = 0; i < m_paramCount; ++i) {

            QVariant parameter;
            stream >> parameter;
            m_parameters.append(parameter);
            m_args.append((void*)m_parameters.at(i).data());
        }
    }
    bool call() {

        return m_service->emitForwardedSignal(m_signal,m_args.data());
    }

    SignalCall(const SignalCall& s) {

        m_args = s.m_args;
        m_service = s.m_service;
        m_signal = s.m_signal;
        m_slot = s.m_slot;
        m_paramCount = s.m_paramCount;
        m_key = s.m_key;
        m_parameters = s.m_parameters;
    }
    ~SignalCall(){}
private:
    QVector<void*> m_args;
    MetaService* m_service;
    int m_signal;
    int m_slot;
    int m_paramCount;
    QUuid m_key;
    QVariantList m_parameters;
};
Q_DECLARE_METATYPE(SignalCall)

class ServerLinkRequestHandler : public RequestHandler {

public:
    ServerLinkRequestHandler(Controller* controller);
    virtual ~ServerLinkRequestHandler();

protected:
    virtual void handle(Packet&);
};

class ServerDisconnectRequestHandler : public RequestHandler {

public:
    ServerDisconnectRequestHandler(Controller* controller);
    virtual ~ServerDisconnectRequestHandler();

protected:
    virtual void handle(Packet&);
};

class QTimer;
class ServerSignalForwardRequestHandler : public RequestHandler {

    Q_OBJECT

public:
    ServerSignalForwardRequestHandler(Controller* controller);
    virtual ~ServerSignalForwardRequestHandler();

protected:
    virtual void handle(Packet &);
private slots:
    void dequeueSignal();
private:
    QQueue<SignalCall> m_signalQueue;
    QTimer* m_scheduler;
};

class ServerSignalConnectionRequestHandler : public ServerSignalForwardRequestHandler {

public:
    ServerSignalConnectionRequestHandler(Controller* controller);
    virtual ~ServerSignalConnectionRequestHandler();
protected:
    virtual void handle(Packet&);

};

class ServerMethodCallRequestHandler : public RequestHandler {

public:
    ServerMethodCallRequestHandler(Controller* controller);
    virtual ~ServerMethodCallRequestHandler();

protected:
    virtual void handle(Packet &);

};

class ServerLinkConfirmationHandler : public ConfirmationHandler {

public:
    ServerLinkConfirmationHandler(Controller* controller);
    virtual ~ServerLinkConfirmationHandler();
protected:
    void handle(Packet &);
};

class ServerMethodCallConfirmationHandler : public ConfirmationHandler {

public:
    ServerMethodCallConfirmationHandler(Controller* controller);
    virtual ~ServerMethodCallConfirmationHandler();
protected:
    void handle(Packet &);
};

class ServerSignalForwardConfirmationHandler : public ConfirmationHandler {

public:
    ServerSignalForwardConfirmationHandler(Controller* controller);
    virtual ~ServerSignalForwardConfirmationHandler();
protected:
    virtual void handle(Packet &);
};

class ServerSignalConnectionConfirmationHandler : public ServerSignalForwardConfirmationHandler {

public:
    ServerSignalConnectionConfirmationHandler(Controller* controller);
    ~ServerSignalConnectionConfirmationHandler();
protected:
    virtual void handle(Packet &);
};

class ServerDisconnectConfirmationHandler : public ConfirmationHandler {

public:
    ServerDisconnectConfirmationHandler(Controller* controller);
    ~ServerDisconnectConfirmationHandler();
protected:
    virtual void handle(Packet &);
};

class ServerHandlerFactory : public AbstractHandlerFactory {

    ServerHandlerFactory();

public:
    ~ServerHandlerFactory();
    static AbstractHandlerFactory* instance() {

        if (!s_instance) {
            s_instance = new ServerHandlerFactory();
        }
        return s_instance;
    }
    virtual bool addPair(HandlerType request, HandlerType response, Controller* controller);

private:
    static AbstractHandlerFactory* s_instance;
    Handler* createRequest(HandlerType request, Controller* controller);
    Handler* createResponse(HandlerType response, Controller* controller);
};
#endif // SERVER_HANDLER_H
