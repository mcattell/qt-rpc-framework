#include "rpcclient.h"
#include "rpcclient_p.h"
#include "client_metaservice.h"
#include "rpc_global.h"
#include "client_controller.h"

RpcClient::RpcClient(QObject *parent)
    : QObject(parent)
    ,d_ptr(new RpcClientPrivate(this)){

    qRegisterMetaType<Rpc::Status>("Rpc::RpcStatus");
    qRegisterMetaType<Rpc::BlockingMode>("Rpc::BlockingMode");
    qRegisterMetaType<Packet::PacketHeader>("Packet::PacketHeader");
    qRegisterMetaType<Packet>("Packet");
}

RpcClient::~RpcClient() {
}


RpcClientPrivate::RpcClientPrivate(QObject *parent)
    : QObject(parent) {
}

RpcClientPrivate::~RpcClientPrivate() {
}

QUuid RpcClient::bind(const QByteArray& className, const QString& hostAddress, qint16 remotePort) {

    Q_D(RpcClient);

    if (!d->scanForExisting(remotePort)) {

        MetaService* service = new ClientMetaService(hostAddress, remotePort, QUuid(), className);
        if (service->bind(_SIG_SLOT_BINDING_)) {
            connect(service->controller(),SIGNAL(stateChanged(Controller::ControllerState,QUuid))
                    , d
                    , SLOT(on_ControllerStateChanged(Controller::ControllerState,QUuid)),Qt::QueuedConnection);
            connect(d, SIGNAL(statusChanged(Rpc::Status,QUuid)),this,SIGNAL(clientStatusChanged(Rpc::Status,QUuid)));
            d->m_serviceMap.insert(service->uuid(),service);
            return service->uuid();
        }
        return QUuid();
    }
    return QUuid();
}

bool RpcClient::bind(QObject* client, const QString& hostAddress, qint16 remotePort) {

    Q_D(RpcClient);

    if (!d->scanForExisting(remotePort)) {

        MetaService* service = new ClientMetaService(client
                                                     , hostAddress
                                                     , remotePort
                                                     , QUuid()
                                                     , QByteArray(client->metaObject()->className())
                                                     );
        if (service->bind(_RPC_BINDING_)) {

            connect(service->controller(),SIGNAL(stateChanged(Controller::ControllerState,QUuid))
                    , d
                    , SLOT(on_ControllerStateChanged(Controller::ControllerState,QUuid)));
            d->m_serviceMap.insert(service->uuid(),service);
            connect(d, SIGNAL(statusChanged(Rpc::Status,QUuid)),this,SIGNAL(clientStatusChanged(Rpc::Status,QUuid)));
            return true;
        }
        return false;
    }
    return false;
}

bool RpcClient::connectSignalToRemoteSlot(QObject *source
                                          , const QByteArray& signal
                                          , const QByteArray& slot
                                          , const QUuid& remoteId
                                          , Rpc::BlockingMode mode ) {
    Q_D(RpcClient);
    if (d->m_serviceMap.value(remoteId)->subject()) {
        qWarning("You are trying to connect to a remote object that is registered for remote method calls only.");
        return false;
    }

    return d->m_serviceMap.value(remoteId)->connectSignalToRemoteSlot(source,signal,slot,mode);

}

bool RpcClient::connectRemoteSignalToSlot(const QUuid& remoteObjectId
                                          , int signal
                                          , QObject* target
                                          , int slot
                                          , Rpc::BlockingMode mode) {

    Q_D(RpcClient);

    if (d->m_serviceMap.value(remoteObjectId)->subject()) {

        qWarning("You are trying a signal on a remote object to an object that is registered for remote method calls only.");
        return false;
    }

    return d->m_serviceMap.value(remoteObjectId)->connectRemoteSignalToSlot(signal,target,slot,mode);
}

bool RpcClientPrivate::scanForExisting(qint16 port) {

    QMapIterator<QUuid, MetaService *> i(m_serviceMap);
    while (i.hasNext()) {

        i.next();
        if (i.value()->port() == port) {

            return true;
        }
    }
    return false;
}
void RpcClientPrivate::on_ControllerStateChanged(Controller::ControllerState state, const QUuid &id) {

    switch (state) {
    case Controller::ERROR:
    case Controller::UNKNOWN: {
       emit statusChanged(Rpc::INACTIVE,id);
    }break;
    default: break;
    }
}



