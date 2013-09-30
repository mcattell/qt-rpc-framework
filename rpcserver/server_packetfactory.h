#ifndef SERVERPACKETFACTORY_H
#define SERVERPACKETFACTORY_H

#include "packet.h"

class ServerPacketFactory : public AbstractPacketFactory
{
public:

    static AbstractPacketFactory* instance();
    ~ServerPacketFactory();
    virtual void create(PacketIdType type, MetaService *service, Packet& packet);

private:
    ServerPacketFactory();
    static AbstractPacketFactory* s_instance;

    void createLinkConfirmationPacket(Packet&, MetaService*);
    void createSignalConnectConfirmationPacket(Packet&);
    void createSignalForwardConfirmationPacket(Packet&);
    void createCallMethodConfirmationPacket(Packet&);
    void createDisconnectConfirmationPacket(Packet&);
};

#endif // SERVERPACKETFACTORY_H
