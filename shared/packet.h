#ifndef PACKET_H
#define PACKET_H
#include <QtCore/QUuid>
#include <QtCore/QByteArray>
#include <QtCore/QDataStream>
#include "rpc_global.h"

#define BEGIN_SERIALIZER QByteArray data; \
    QDataStream stream(&data,QIODevice::WriteOnly);

#define END_SERIALIZER packet.setPayload(data);

#define BEGIN_DESERIALIZER \
    QByteArray data = packet.getPayload(); \
    QDataStream stream(&data,QIODevice::ReadOnly);


#define END_DESERIALIZER

using namespace RpcPrivate;
struct Packet {

    static int s_sequence;

    struct  PacketHeader{

        PacketHeader(PacketSizeType s = 0
                , PacketIdType id = 0
                , PacketType t = 0
                , PacketMessageType m = 0
                , Synchronicity syn = _ASYNCHRONOUS_
                )
            : size(s)
            ,pid(id)
            ,type(t)
            ,msg(m)
            ,sync(syn) {
        }
        ~PacketHeader() {}

        PacketSizeType        size;
        PacketIdType          pid;
        PacketType            type;//request or confirmation
        PacketMessageType     msg;
        Synchronicity         sync;

        PacketHeader(const PacketHeader& a) {
            copy(a);
        }
        PacketHeader operator=(const PacketHeader& a) {
            copy(a);
            return *this;
        }
        void copy(const PacketHeader& a) {
            size = a.size;
            pid = a.pid;
            type = a.type;
            msg = a.msg;
            sync = a.sync;
        }
    };

    int sizeOf() {
        return (sizeof(PacketIdType)
                + sizeof(PacketType)
                + sizeof(PacketMessageType));
    }

    Packet() {
    }
    Packet(const Packet& rhs){
        copy(rhs);
    }
    Packet operator=(const Packet& rhs) {
        copy(rhs);
        return *this;
    }
    ~Packet(){}

private:

    void copy(const Packet& p) {

        m_header = p.m_header;
        m_data = p.m_data;
        m_userData = p.m_userData;
        m_key = p.m_key;
    }

    PacketHeader m_header;
    QByteArray m_data;
    QUuid m_key;
    void* m_userData;

public:
    static int sequence() {
        return ++s_sequence;
    }
    inline const QByteArray& getPayload() const {
        return m_data;
    }
    inline void setPayload(const QByteArray& payload) {
        m_data = payload;
        m_header.size = sizeOf() + m_data.size();
    }
    inline PacketType getType() const {
        return m_header.type;
    }
    inline void setType(PacketType type) {
        m_header.type = type;
    }
    inline PacketIdType getPid() const {
        return m_header.pid;
    }
    inline void setPid(PacketIdType id) {
        m_header.pid = id;
    }
    inline PacketSizeType size() const {
        return m_header.size;
    }
    inline void setSize(PacketSizeType size) {
        m_header.size = size;
    }
    inline PacketMessageType getMsg() const {
        return m_header.msg;
    }
    inline void setMsg(PacketMessageType msg) {
        m_header.msg = msg;
    }
    inline void setUserData(void* data) {
        m_userData = data;
    }
    inline void* getUserData() const {
        return m_userData;
    }
    inline void setKey(const QUuid& key) {
        m_key = key;
    }
    inline const QUuid& key() const {
        return m_key;
    }
    inline void setSync(Synchronicity sync) {
        m_header.sync = sync;
    }
    inline Synchronicity sync() const {
        return m_header.sync;
    }
    friend const QDataStream& operator>>(QDataStream& stream, Packet& packet) {

        int pid, type, msg, sync;

        stream >> pid >> type >> msg >> sync;

        packet.setPid(pid);
        packet.setType(type);
        packet.setMsg(msg);

        if (packet.size() > packet.sizeOf()) {

            QByteArray data;
            stream >> data;
            packet.setPayload(data);
        } else {

            packet.setSize(packet.sizeOf());
        }
        return stream;
    }
    friend QDataStream& operator<<(QDataStream& stream, const Packet& packet) {

        stream << packet.size() << packet.getPid() << packet.getType() << packet.getMsg() << packet.sync();
        QByteArray payload(packet.getPayload());
        stream << payload;
        return stream;
    }
};

Q_DECLARE_METATYPE(Packet::PacketHeader)
Q_DECLARE_METATYPE(Packet)

class MetaService;
struct AbstractPacketFactory {

    virtual void create(PacketIdType type, MetaService* service, Packet&) = 0;

};

#endif // PACKET_H
