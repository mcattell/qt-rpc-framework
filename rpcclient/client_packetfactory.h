#ifndef CLIENTPACKETFACTORY_H
#define CLIENTPACKETFACTORY_H
#include "packet.h"
#include "client_metaservice.h"
#include "client_controller.h"

class ClientPacketFactory : public AbstractPacketFactory
{
public:
    virtual void create(PacketIdType type, MetaService* service, Packet&);
    static AbstractPacketFactory* instance();
    ~ClientPacketFactory();
protected:
    ClientPacketFactory();
private:
    static AbstractPacketFactory* s_instance;

    void createLinkRequestPacket(Packet& packet, MetaService* service);
    void createSignalConnectPacket(Packet& packet, MetaService* service);
    void createSignalForwardPacket(Packet& packet, MetaService* service);
    void createCallMethodPacket(Packet& packet, MetaService* service);
    void createDisconnectRequestPacket(Packet& packet);
};

#endif // CLIENTPACKETFACTORY_H
