#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QtCore/QObject>
#include <QtCore/QUuid>
#include "rpc_global.h"

class Packet;
class Handler;
class Protocol;
class MetaService;
class Link;

class Controller : public QObject {

    Q_OBJECT
public:
    typedef enum {
        IDLE,
        READY,
        LINKING,
        LINKED,
        BUSY,
        ERROR,
        UNKNOWN
    }ControllerState;


    Controller(QObject* parent, MetaService* service)
        : QObject(parent)
        , m_metaService(service){
        qRegisterMetaType<Controller::ControllerState>("Controller::ControllerState");
        m_state = IDLE;
    }
    ~Controller(){}

    virtual void process(Packet&) = 0;
    virtual bool waitFor(Packet&) = 0;
    virtual void confirm(Packet&) = 0;
    virtual void setProtocol(Protocol*) = 0;
    virtual Protocol* protocol() = 0;
    virtual Handler* response(Handler*) = 0;
    virtual Link* link() const = 0;

    MetaService* metaService() const;
    virtual ControllerState state() const;
    virtual void setState(ControllerState state);

signals:
    void stateChanged(Controller::ControllerState state, const QUuid& id);

private:
    ControllerState m_state;
    MetaService* m_metaService;
};
Q_DECLARE_METATYPE(Controller::ControllerState)
#endif // CONTROLLER_H
