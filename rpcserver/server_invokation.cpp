#include "server_invokation.h"

ServerInvokation::ServerInvokation(MetaService *parent, int signalId, int slotId, const QList<QByteArray> &parameterTypes) {

    Q_UNUSED(parent);
    Q_UNUSED(signalId);
    Q_UNUSED(slotId);
    Q_UNUSED(parameterTypes);
}
ServerInvokation::~ServerInvokation() {
}
void ServerInvokation::call(QObject *sender, void **arguments) {

    Q_UNUSED(sender);
    Q_UNUSED(arguments);
}

const int& ServerInvokation::signalId() const {

    return m_signalId;
}
const int& ServerInvokation::slotId() const {

    return m_slotId;
}
const QVariantList& ServerInvokation::parameters() const {

    return m_parameters;
}
