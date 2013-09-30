#include <QtCore/QThread>
#include "server_metaservice.h"
#include "daemon.h"
#include "server_controller.h"
#include "server_invokation.h"

Uuid ServerMetaService::s_RpcNamespace = {0,0,0,{0}};

ServerMetaService::ServerMetaService(QObject* subject, qint16 port, const QUuid& uuid, const QByteArray& className)
    : MetaService(subject, port, uuid, className)
    , m_thread(new QThread())
    , m_server(new Daemon()) {

    m_server->setPort(port);
    m_server->moveToThread(m_thread);

    setController(new ServerController(this,this));

    QObject::connect(m_server,SIGNAL(notifyConnection(qintptr)),static_cast<ServerController*>(controller()),SLOT(acceptConnection(qintptr)));
    QObject::connect(thread(),SIGNAL(started()),server(),SLOT(start()));
    setInventory(this->takeInventory());
    this->moveToThread(m_thread);
    thread()->start();
}
ServerMetaService::ServerMetaService(const ServerMetaService &rhs)
    : MetaService(rhs)
    ,m_thread(rhs.thread())
    ,m_server(rhs.server()) {
}
ServerMetaService::~ServerMetaService() {
}

QUuid ServerMetaService::createUuidFromString(const QString &identifier)  {

    const QUuid ns(s_RpcNamespace.data1, s_RpcNamespace.data2, s_RpcNamespace.data3,
                   s_RpcNamespace.data4[0], s_RpcNamespace.data4[1], s_RpcNamespace.data4[2],
                   s_RpcNamespace.data4[3], s_RpcNamespace.data4[4], s_RpcNamespace.data4[5],
                   s_RpcNamespace.data4[6], s_RpcNamespace.data4[7]);

    return QUuid::createUuidV3(ns, identifier);
}

void ServerMetaService::initUuidNamespace() {

    s_RpcNamespace.data1 = 0x6ba7b810;
    s_RpcNamespace.data2 = 0x9dad;
    s_RpcNamespace.data3 = 0x11d1;
    s_RpcNamespace.data4[0] = 0x80;
    s_RpcNamespace.data4[1] = 0xb4;
    s_RpcNamespace.data4[2] = 0x00;
    s_RpcNamespace.data4[3] = 0xc0;
    s_RpcNamespace.data4[4] = 0x4f;
    s_RpcNamespace.data4[5] = 0xd4;
    s_RpcNamespace.data4[6] = 0x30;
    s_RpcNamespace.data4[7] = 0xc8;
}

bool ServerMetaService::bind(BindingType type) {

    setBindingType(type);
    controller()->setState(Controller::LINKED);
    return (controller()->state() == Controller::LINKED);
}

QUuid ServerMetaService::targetUuid() const {

    return MetaService::uuid();
}
QThread* ServerMetaService::thread() const {

    return m_thread;
}
Daemon* ServerMetaService::server() const {

    return m_server;
}
bool ServerMetaService::connectSignalToRemoteSlot(QObject *source
                                                  , const QByteArray& signal
                                                  , const QByteArray& slot
                                                  , Rpc::BlockingMode mode) {
    Q_UNUSED(source);
    Q_UNUSED(signal);
    Q_UNUSED(slot);
    Q_UNUSED(mode);
    return false;
}

bool ServerMetaService::connectRemoteSignalToSlot(int signal
                                                  , QObject* target
                                                  , int slot
                                                  , Rpc::BlockingMode mode) {

    Qt::ConnectionType connectionType;
    connectionType = (mode == Rpc::Synchronous) ? Qt::DirectConnection : Qt::AutoConnection;

    if (slot < 0)
        return false;

    m_connections.insert(signal, new ServerInvokation(this, signal, slot, target->metaObject()->method(slot).parameterTypes()));

    return QMetaObject::connect(this, signal + inventory().offset, target, slot, connectionType);
}

bool ServerMetaService::emitForwardedSignal(int signal, void **args) {

    if (signal >= 0) {

        QMetaObject::activate(this, metaObject(), signal + inventory().offset, args);
        return true;
    } else {

        return false;
    }
}

int ServerMetaService::qt_metacall(QMetaObject::Call c, int id, void **arguments) {

   return subject()->qt_metacall(c, id, arguments);
}
Inventory ServerMetaService::takeInventory() {

    const QMetaObject* metaObject =  subject()->metaObject();

    Inventory list;
    list.offset = metaObject->methodOffset();
    int size = metaObject->methodCount();

    for (int count = 0; count < size; ++count) {

        if (count >= list.offset) {

            if (metaObject->method(count).methodType() == QMetaMethod::Method) {

                MethodDescriptor method;
                method.name = metaObject->method(count).methodSignature();
                method.id = count;
                method.paramCount = metaObject->method(count).parameterCount();
                method.ret = metaObject->method(count).returnType();
                list.methodList.append(method);


            } else if (metaObject->method(count).methodType() == QMetaMethod::Slot) {

                SlotDescriptor slot;
                slot.name = metaObject->method(count).methodSignature();
                slot.id = count;
                slot.paramCount = metaObject->method(count).parameterCount();
                list.slotList.append(slot);

            } else if (metaObject->method(count).methodType() == QMetaMethod::Signal) {

                SignalDescriptor signal;
                signal.name = metaObject->method(count).methodSignature();
                signal.id = count;
                signal.paramCount = metaObject->method(count).parameterCount();
                list.signalList.append(signal);

            } else {

                qWarning("Unknown method type.");
            }
        }
    }
    list.signalCount = list.signalList.size();
    list.methodCount = list.methodList.size();
    list.slotCount = list.slotList.size();

    return list;
}
Invokation* ServerMetaService::createInvokation(int signalId, int slotId, const QList<QByteArray>& parameterTypes) {

    return new ServerInvokation(this, signalId, slotId, parameterTypes);
}
const NumericSignalSlotPair& ServerMetaService::numericSignalSlot() const {

    return m_numericSignalSlotPair;
}
const StringSignalSlotPair& ServerMetaService::stringSignalSlot() const {

    return m_stringSignalSlotPair;
}
