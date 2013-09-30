#ifndef RPCCLIENT_P_H
#define RPCCLIENT_P_H
#include <QtCore/QObject>
#include <QtCore/QUuid>
#include <QtCore/QMap>
#include "rpcclient.h"
#include "controller.h"

class MetaService;

class RpcClientPrivate : public QObject{

     Q_OBJECT
     Q_DECLARE_PUBLIC(RpcClient)

public:
    RpcClientPrivate(QObject* parent = 0);
    ~RpcClientPrivate();

    bool scanForExisting(qint16 port);
    QMap<QUuid,MetaService*> m_serviceMap;


public slots:
    void on_ControllerStateChanged(Controller::ControllerState state, const QUuid& id);

signals:
    void statusChanged(Rpc::Status status, const QUuid& target);
private:
    RpcClient* q_ptr;
};
#endif // RPCCLIENT_P_H
