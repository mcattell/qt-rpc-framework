#include <QtCore/QDebug>
#include "client_metaservice.h"
#include "client_controller.h"
#include "packet.h"
#include "client_packetfactory.h"
#include "client_invokation.h"

ClientMetaService::ClientMetaService(QObject* subject
                                     , const QString& hostAddress
                                     , qint16 port
                                     , const QUuid& uuid
                                     , const QByteArray& className)
    : MetaService( subject
                   ,port
                   ,uuid
                   ,className)
    , m_hostAddress(hostAddress){

    setController(new ClientController(this, this));
}
ClientMetaService::ClientMetaService(const QString &hostAddress, qint16 port
                                     , const QUuid &uuid
                                     , const QByteArray &className)
    : MetaService( 0
                   ,port
                   , uuid
                   ,className)
    , m_hostAddress(hostAddress){

    setController(new ClientController(this, this));
}

ClientMetaService::ClientMetaService(const ClientMetaService &rhs)
    : MetaService(rhs) {
}
ClientMetaService::~ClientMetaService() {
}


bool ClientMetaService::bind(BindingType type) {

    setBindingType(type);
    Packet out;
    ClientPacketFactory::instance()->create(_REQUEST_LINK_, this, out);
    controller()->process(out);
    return controller()->waitFor(out);
}

bool ClientMetaService::connectSignalToRemoteSlot(QObject *source
                                                  , const QByteArray& signal
                                                  , const QByteArray& slot
                                                  , Rpc::BlockingMode mode) {

    if (controller()->state() == Controller::LINKED) {
        QByteArray theSignal = QMetaObject::normalizedSignature(signal);
        QByteArray theSlot = QMetaObject::normalizedSignature(slot);

        if (!QMetaObject::checkConnectArgs(theSignal, theSlot))
            return false;

        int signalId = source->metaObject()->indexOfSignal(theSignal);
        if (signalId < 0)
            return false;

        int slotId = inventory().idForName(theSlot, QMetaMethod::Slot);


        if (slotId < 0) {
            qWarning("Unable to find the requested slot in the remote object inventory");
        }

        m_numericSignalSlotPair.sigId = signalId;
        m_numericSignalSlotPair.slotId = slotId;
        m_numericSignalSlotPair.mode = mode;
        m_stringSignalSlotPair.sigName = theSignal;
        m_stringSignalSlotPair.slotName = theSlot;
        m_stringSignalSlotPair.mode = mode;


        m_connections.insert(slotId, createInvokation(signalId, slotId, source->metaObject()->method(signalId).parameterTypes()));

        Packet request;
        ClientPacketFactory::instance()->create(_SIGNAL_CONNECT_, this, request);
        controller()->process(request);
        controller()->waitFor(request);

        return QMetaObject::connect(source, signalId, this, slotId);
    }
    return false;
}

bool ClientMetaService::connectRemoteSignalToSlot(int signal
                                                  , QObject* target
                                                  , int slot
                                                  , Rpc::BlockingMode mode) {

    Q_UNUSED(signal);
    Q_UNUSED(target);
    Q_UNUSED(slot);
    Q_UNUSED(mode);

    return false;
}
int ClientMetaService::qt_metacall(QMetaObject::Call c, int id, void **arguments) {

    if (controller()->state() == Controller::LINKED) {
        if (id < 0 || c != QMetaObject::InvokeMetaMethod)
            return id;
        m_currentInvokation = m_connections.value(id);
        m_currentInvokation->call(sender(), arguments);
    }
    return -1;
}
Inventory ClientMetaService::takeInventory() {

    return Inventory();
}
/************************************************************************************
 * Create an invokation for a signal to be forwarded to a remote slot               *
 ************************************************************************************/
Invokation* ClientMetaService::createInvokation(int signalId
                                                , int slotId
                                                , const QList<QByteArray>& parameterTypes) {

    return  new ClientInvokation(this, signalId, slotId, 0, 0, parameterTypes);
}


const NumericSignalSlotPair& ClientMetaService::numericSignalSlot() const {

    return m_numericSignalSlotPair;
}
const StringSignalSlotPair& ClientMetaService::stringSignalSlot() const {

    return m_stringSignalSlotPair;
}
bool ClientMetaService::emitForwardedSignal(int signal, void **args) {

    Q_UNUSED(signal);
    Q_UNUSED(args);

    return false;
}
Invokation* ClientMetaService::invokation() const {

    return m_currentInvokation;
}
const QHostAddress& ClientMetaService::hostAddress() const {

    return m_hostAddress;
}
