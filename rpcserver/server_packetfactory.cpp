#include "server_packetfactory.h"
#include "server_metaservice.h"
#include "server_controller.h"
#include "rpc_global.h"

AbstractPacketFactory* ServerPacketFactory::s_instance = 0;

ServerPacketFactory::ServerPacketFactory(){
}
ServerPacketFactory::~ServerPacketFactory() {
}
AbstractPacketFactory* ServerPacketFactory::instance() {

    if (!s_instance) {

        s_instance = new ServerPacketFactory();
    }
    return s_instance;
}

void ServerPacketFactory::create(PacketIdType type, MetaService *service, Packet& packet) {


    packet.setType(_CONFIRMATION_);
    packet.setMsg(type);

    switch (type) {

    case _CONFIRM_LINK_: {

        createLinkConfirmationPacket(packet, service);
    }break;
    case _SIGNAL_CONF_CONN_: {

        createSignalConnectConfirmationPacket(packet);
    }break;
    case _SIGNAL_CONF_: {

        createSignalForwardConfirmationPacket(packet);
    }break;
    case _RETURN_FROM_CALL_: {

        createCallMethodConfirmationPacket(packet);
    }break;
    case _CONFIRM_DISCONNECT_: {

        createDisconnectConfirmationPacket(packet);
    }break;
    }
}
void ServerPacketFactory::createLinkConfirmationPacket(Packet& packet, MetaService* service) {

    BEGIN_SERIALIZER
    packet.setSize(_SYNCHRONOUS_);
    stream << service->uuid() << *service;
    END_SERIALIZER
}

void ServerPacketFactory::createSignalConnectConfirmationPacket(Packet& packet) {
    Q_UNUSED(packet);
}

void ServerPacketFactory::createSignalForwardConfirmationPacket(Packet& packet) {
    Q_UNUSED(packet);
}

void ServerPacketFactory::createCallMethodConfirmationPacket(Packet& packet) {
    Q_UNUSED(packet);
}

void ServerPacketFactory::createDisconnectConfirmationPacket(Packet& packet) {
    Q_UNUSED(packet);
}
