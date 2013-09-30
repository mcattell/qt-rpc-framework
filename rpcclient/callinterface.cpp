#include "client_controller.h"
#include "callinterface.h"
#include "client_metaservice.h"
#include "client_packetfactory.h"

CallInterfaceFactory* CallInterfaceFactory::s_instance = 0;
QHash<QObject*, CallInterface*> CallInterfaceFactory::s_interfaces = QHash<QObject*, CallInterface*>();

CallInterfaceFactory::CallInterfaceFactory() {
}
CallInterfaceFactory::~CallInterfaceFactory() {
}
ClientCallInterface::ClientCallInterface(Controller* controller)
    : m_controller(controller) {
}

CallInterfaceFactory* CallInterfaceFactory::instance() {

    if (!s_instance) {
        s_instance = new CallInterfaceFactory();
    }
    return s_instance;
}
CallInterface* CallInterfaceFactory::create(Controller *controller) {

    CallInterface* intf = new ClientCallInterface(controller);
    s_interfaces.insert(controller->metaService()->subject(), intf);
    return intf;
}

CallInterface* CallInterfaceFactory::instance(QObject *subject) {

    return s_interfaces.value(subject);
}

bool ClientCallInterface::call(methodData_t *data) {

    Packet outgoing;
    outgoing.setUserData((void*)data);
    ClientPacketFactory::instance()->create(_CALL_METHOD_, m_controller->metaService(), outgoing);

    m_controller->process(outgoing);
    return m_controller->waitFor(outgoing);
}
