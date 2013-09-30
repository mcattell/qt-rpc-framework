#ifndef RPCCLIENT_H
#define RPCCLIENT_H
#include "rpcclient_global.h"
#include "rpc_global.h"
#include "controller.h"
#include <QtCore/QObject>
#include <QtCore/QUuid>


class RpcClientPrivate;
class RPCCLIENTSHARED_EXPORT RpcClient : public QObject
{

    Q_OBJECT

public:
    RpcClient(QObject* parent = 0);
    ~RpcClient();

    QUuid bind(const QByteArray& className, const QString& hostAddress, qint16 remotePort);
    bool bind(QObject* client, const QString& hostAddress, qint16 remotePort);
    bool connectSignalToRemoteSlot(QObject *source
                           , const QByteArray& signal
                           , const QByteArray& slot
                           , const QUuid& remoteId
                           , Rpc::BlockingMode mode = Rpc::Asynchronous);

    bool connectRemoteSignalToSlot(const QUuid& remoteObjectId
                             , int remoteSignal
                             , QObject* target
                             , int slot
                             , Rpc::BlockingMode = Rpc::Asynchronous);
signals:
    void clientStatusChanged(Rpc::Status status, const QUuid& target);

private:

    Q_DECLARE_PRIVATE(RpcClient)
    Q_DISABLE_COPY(RpcClient)
    RpcClientPrivate* d_ptr;
};
#endif // RPCCLIENT_H
