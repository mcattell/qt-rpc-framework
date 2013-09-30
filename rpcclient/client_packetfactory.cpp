#include <QtCore/QDebug>
#include "client_packetfactory.h"
#include "client_metaservice.h"
#include "client_invokation.h"
#include "callinterface.h"

AbstractPacketFactory* ClientPacketFactory::s_instance = 0;

AbstractPacketFactory* ClientPacketFactory::instance() {
    if (!s_instance) {
        s_instance = new ClientPacketFactory();
    }
    return s_instance;
}

ClientPacketFactory::ClientPacketFactory(){
}
ClientPacketFactory::~ClientPacketFactory(){
}
void ClientPacketFactory::create(PacketIdType type, MetaService *service, Packet& packet) {



    packet.setType(_REQUEST_);
    packet.setPid(Packet::sequence());
    packet.setMsg(type);

    switch (type) {

    case _REQUEST_LINK_: {
        createLinkRequestPacket(packet, service);
    }break;
    case _SIGNAL_CONNECT_: {
        createSignalConnectPacket(packet, service);
    }break;
    case _SIGNAL_FORWARD_: {
        createSignalForwardPacket(packet, service);
    }break;
    case _CALL_METHOD_: {
        createCallMethodPacket(packet, service);
    }break;
    case _REQUEST_DISCONNECT_: {
        createDisconnectRequestPacket(packet);
    }break;
    }
}

void ClientPacketFactory::createLinkRequestPacket(Packet& packet, MetaService *service) {

    packet.setSync(_SYNCHRONOUS_);

    BEGIN_SERIALIZER
            stream << service->className() << (int)service->bindingType();
    END_SERIALIZER
}

void ClientPacketFactory::createSignalConnectPacket(Packet& packet, MetaService* service) {

    packet.setSync(_SYNCHRONOUS_);

    BEGIN_SERIALIZER
    stream << service->uuid()
           << service->numericSignalSlot().sigId
           << service->numericSignalSlot().slotId
           << (int)service->stringSignalSlot().mode;
    END_SERIALIZER
}

void ClientPacketFactory::createSignalForwardPacket(Packet& packet, MetaService* service) {

    packet.setSync(_ASYNCHRONOUS_);

    Invokation* invokation = static_cast<ClientMetaService*>(service)->invokation();
    int paramCount = invokation->parameters().size();
    BEGIN_SERIALIZER
    stream << service->uuid()
           << invokation->signalId()
           << invokation->slotId()
           << paramCount;
    const QVariantList& parameters = invokation->parameters();
    for (int i = 0; i < paramCount; ++i) {

        stream << parameters.at(i);
    }
    END_SERIALIZER

}


void ClientPacketFactory::createCallMethodPacket(Packet& packet, MetaService *service) {

    packet.setSync(_SYNCHRONOUS_);

    methodData_t* method = reinterpret_cast<methodData_t*>(packet.getUserData());

    BEGIN_SERIALIZER
            stream << service->uuid() << *method;
    END_SERIALIZER
}

void ClientPacketFactory::createDisconnectRequestPacket(Packet& packet) {

    Q_UNUSED(packet);
}
