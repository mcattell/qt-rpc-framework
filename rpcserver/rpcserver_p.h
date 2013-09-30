#ifndef RPCSERVER_P_H
#define RPCSERVER_P_H
#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QUuid>
#include "rpcserver.h"
#include "server_controller.h"

class Daemon;
class QThread;
class MetaService;

typedef QMap<QUuid,MetaService*> ServiceMap;
typedef QMap<MetaService*,Controller*> ServiceControllerMap;

class RpcServerPrivate : public QObject {

    Q_OBJECT
    Q_DECLARE_PUBLIC(RpcServer)
public:
    RpcServerPrivate(QObject* parent = 0);
    bool register_receiver(QObject*, qint16 port);
    bool disconnect(QUuid id);
public slots:
    void onStatusChanged(Controller::ControllerState state, QUuid uuid);
signals:
    void notifyStatusChange(Rpc::Status status, QObject* object);
private:
    ServiceMap m_servicesMap;
    RpcServer* q_ptr;
};

#endif // RPCSERVER_P_H
