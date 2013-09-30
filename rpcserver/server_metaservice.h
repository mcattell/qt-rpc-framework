#ifndef SERVERMETASERVICE_H
#define SERVERMETASERVICE_H
#include "metaservice.h"

class QThread;
class Daemon;

typedef struct
{
    uint    data1;
    ushort  data2;
    ushort  data3;
    uchar   data4[8];
}Uuid;

class ServerMetaService : public MetaService
{

public:
    ServerMetaService(QObject* subject, qint16 port, const QUuid& uuid, const QByteArray& className);
    ServerMetaService(const ServerMetaService& rhs);
    virtual ~ServerMetaService();

    static Uuid s_RpcNamespace;
    static QUuid createUuidFromString(const QString &);
    static void initUuidNamespace();

    virtual bool bind(BindingType type);
    virtual QUuid targetUuid() const;
    QThread* thread() const;

    virtual bool connectSignalToRemoteSlot(QObject *source
                                   , const QByteArray& signal
                                   , const QByteArray& slot
                                   , Rpc::BlockingMode mode);

    virtual bool connectRemoteSignalToSlot(int remoteSignal
                                       , QObject* target
                                       , int slot
                                       , Rpc::BlockingMode mode);
    virtual bool emitForwardedSignal(int signal, void **args);
    virtual int qt_metacall(QMetaObject::Call c, int id, void **arguments);
    virtual Inventory takeInventory();
    virtual Invokation* createInvokation(int signalId, int slotId, const QList<QByteArray>& parameterTypes);
    virtual const NumericSignalSlotPair& numericSignalSlot() const;
    virtual const StringSignalSlotPair& stringSignalSlot() const;

protected:
    Daemon* server() const;

private:
    QMultiHash<int, Invokation*> m_connections;
    QThread* m_thread;
    Daemon* m_server;
    NumericSignalSlotPair m_numericSignalSlotPair;
    StringSignalSlotPair m_stringSignalSlotPair;
};

#endif // SERVERMETASERVICE_H
