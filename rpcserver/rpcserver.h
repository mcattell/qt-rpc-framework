#ifndef RPCSERVER_H
#define RPCSERVER_H

#include "rpcserver_global.h"
#include <QtCore/QObject>
#include "rpc_global.h"

class Service;
class QUuid;
class RpcServerPrivate;
class RPCSERVERSHARED_EXPORT RpcServer : public QObject
{
    Q_OBJECT

public:
    RpcServer(QObject* parent = 0);
    bool registerReceiver(QObject*,qint16 port);
    bool disconnect(QObject*);

signals:
    void serverStatusChanged(Rpc::Status status, QObject* object);

private:
    RpcServerPrivate* d_ptr;
    Q_DECLARE_PRIVATE(RpcServer)
    Q_DISABLE_COPY(RpcServer)

};

#endif // RPCSERVER_H
