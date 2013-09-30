#include "callinterface.h"
#include "packet.h"
#include "servercontroller.h"


QMultiMap<QObject*,CallInterface*> CallInterfaceFactory::m_interfaceMap = QMultiMap<QObject*,CallInterface*>();
CallInterfaceFactory* CallInterfaceFactory::m_instance = 0;

ServerCallInterface::ServerCallInterface(Controller* controller)
    : m_controller(controller){}

ServerCallInterface::~ServerCallInterface(){}

bool ServerCallInterface::call(methodCall_t* method) {


}

CallInterface* ServerCallInterfaceFactory::create(Controller *controller) {

    CallInterface* interface = new ServerCallInterface(controller);
    m_interfaceMap.insert(static_cast<ServerController*>(controller)->parent(),interface);
    return interface;
}
CallInterfaceFactory* ServerCallInterfaceFactory::instance() {

    if (!m_instance) {
        m_instance = new ServerCallInterfaceFactory();
    }
    return m_instance;
}






