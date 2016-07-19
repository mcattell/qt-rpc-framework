#include <QtCore/QThread>
#include <QtCore/QFile>
#include <QtCore/QStringList>
#include "rpcserver.h"
#include "rpcserver_p.h"
#include "daemon.h"
#include "server_metaservice.h"
#include "server_handler.h"
#include "server_link.h"

RpcServer::RpcServer(QObject* parent)
    :
      QObject(parent)
    ,d_ptr(new RpcServerPrivate(this)){

    qRegisterMetaType<Packet::PacketHeader>("Packet::PacketHeader");
    qRegisterMetaType<Packet>("Packet");
}

RpcServerPrivate::RpcServerPrivate(QObject* parent)
    : QObject(parent){
    q_ptr = static_cast<RpcServer*>(parent);
    ServerMetaService::initUuidNamespace();
    qRegisterMetaType<MetaService*>("MetaService*");
}

bool RpcServer::registerReceiver(QObject * item, qint16 port) {
    Q_D(RpcServer);

    return d->register_receiver(item,port);
}

bool RpcServerPrivate::register_receiver(QObject * obj, qint16 port) {

    Q_ASSERT(obj);
    Q_ASSERT(!obj->objectName().isEmpty());
    QUuid id = ServerMetaService::createUuidFromString(obj->objectName());

    if (m_servicesMap.contains(id)) {

        if (m_servicesMap.value(id)->subject() == obj) {

            qWarning("You have already registered this object, so I won't register it again.");
            return false;
        }
        if (m_servicesMap.value(id)->port() == port) {

            qWarning("An object is already registered at this port.");
            return false;
        }
    }
    MetaService* service = new ServerMetaService(obj, port, id, QByteArray(obj->metaObject()->className()));

    Q_Q(RpcServer);
    if (service) {
        connect(service->controller(),SIGNAL(stateChanged(Controller::ControllerState, QUuid))
                , this
                , SLOT(onStatusChanged(Controller::ControllerState, QUuid)),Qt::QueuedConnection);

        connect(this, SIGNAL(notifyStatusChange(Rpc::Status, QObject*)), q_ptr, SIGNAL(serverStatusChanged(Rpc::Status,QObject*)));
        m_servicesMap.insert(id,service);
        qDebug() << id.toString();
        return static_cast<ServerMetaService*>(m_servicesMap.value(id))->thread()->isRunning();
    }
    return false;
}
void RpcServerPrivate::onStatusChanged(Controller::ControllerState state, QUuid uuid) {

    if (state == Controller::IDLE)
        emit notifyStatusChange(Rpc::INACTIVE, m_servicesMap.value(uuid)->subject());
}
bool RpcServerPrivate::disconnect(QUuid id) {

    m_servicesMap.value(id)->controller()->link()->disconnectFromHost();
}

bool RpcServer::disconnect(QObject * object) {

    QUuid id = ServerMetaService::createUuidFromString(object->objectName());
    Q_D(RpcServer);
    d->disconnect(id);
}

