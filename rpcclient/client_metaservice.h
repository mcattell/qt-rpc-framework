#ifndef CLIENT_METAOBJECT_H
#define CLIENT_METAOBJECT_H
#include <QtNetwork/QHostAddress>
#include "metaservice.h"


class ClientMetaService : public MetaService {

public:
    ClientMetaService(QObject* subject
                      ,const QString& hostAddress
                      ,qint16 port
                      ,const QUuid& uuid
                      ,const QByteArray& className
                      );
    ClientMetaService(const QString& hostAddress
                      ,qint16 port
                      ,const QUuid& uuid
                      ,const QByteArray& className
                      );
    ClientMetaService(const ClientMetaService& rhs);
    virtual ~ClientMetaService();

    virtual bool bind(BindingType type);
    virtual bool connectSignalToRemoteSlot(QObject *source
                                   , const QByteArray& signal
                                   , const QByteArray& slot
                                   , Rpc::BlockingMode mode);
    virtual bool connectRemoteSignalToSlot(int signal
                                       , QObject* target
                                       , int slot
                                       , Rpc::BlockingMode mode);
    virtual bool emitForwardedSignal(int signal, void **args);
    virtual int qt_metacall(QMetaObject::Call c, int id, void **arguments);
    virtual Inventory takeInventory();
    virtual Invokation* createInvokation(int signalId
                                         , int slotId
                                         , const QList<QByteArray>& parameterTypes
                                         );
    void setInvokation(Invokation* invokation);
    Invokation* invokation() const;
    virtual const NumericSignalSlotPair& numericSignalSlot() const;
    virtual const StringSignalSlotPair& stringSignalSlot() const;
    const QHostAddress& hostAddress() const;

private:
    QMultiHash<int, Invokation*> m_connections;
    NumericSignalSlotPair m_numericSignalSlotPair;
    StringSignalSlotPair m_stringSignalSlotPair;
    Invokation* m_currentInvokation;
    QHostAddress m_hostAddress;

};

#endif // CLIENT_METAOBJECT_H
