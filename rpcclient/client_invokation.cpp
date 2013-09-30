#include <QtCore/QObject>
#include <QtCore/QDebug>
#include "client_invokation.h"
#include "client_metaservice.h"
#include "client_packetfactory.h"
#include "packet.h"

ClientInvokation::ClientInvokation(MetaService* parent, int signalId, int slotId, int methodId, int returnType, const QList<QByteArray>& parameterTypes)
    : m_parent(parent)
    , m_parameterTypes(parameterTypes)
    , m_signalId(signalId)
    , m_slotId(slotId)
    , m_methodId(methodId)
    , m_returnType(returnType){
}


ClientInvokation::~ClientInvokation(){
}

void ClientInvokation::call(QObject *sender, void **arguments) {

    Q_UNUSED(sender);
    m_parameters.clear();
    Packet request;
    serialize(arguments);
    ClientPacketFactory::instance()->create(_SIGNAL_FORWARD_,m_parent,request);

    if (m_parent->controller()->state() != Controller::LINKED) {
        qWarning("The client controller is not in the linked state so the call will not be made.");
    } else {
        m_parent->controller()->process(request);
    }
}

const int& ClientInvokation::signalId() const {

    return m_signalId;
}
const int& ClientInvokation::slotId() const {

    return m_slotId;
}
const QVariantList& ClientInvokation::parameters() const {

    return m_parameters;
}
void ClientInvokation::serialize(void** args) {

    int size = m_parameterTypes.size();
    for (int i = 0; i < size; ++i) {

        m_parameters.append(QVariant(QVariant::nameToType(m_parameterTypes.at(i)),args[i+1]));
    }
}
