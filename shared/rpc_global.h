#ifndef RPC_GLOBAL_H
#define RPC_GLOBAL_H
#include <QMetaType>

namespace Rpc {

typedef enum {
    Synchronous,
    Asynchronous
}BlockingMode;

typedef enum {
    ACTIVE,
    INACTIVE
}Status;


} //namespace Rpc
Q_DECLARE_METATYPE(Rpc::Status)
Q_DECLARE_METATYPE(Rpc::BlockingMode)

namespace RpcPrivate {

typedef int PacketSizeType;
typedef int PacketIdType ;
typedef int PacketMessageType;
typedef int PacketType;
typedef int Synchronicity;

#define RPC_TYPE_TO_INT(t) (int)t
#define RPC_PACKET_SIZE_TYPE_TO_INT(t) (PacketSizeType)t
#define RPC_PACKET_ID_TYPE_TO_INT(t) (PacketIdType)t
#define RPC_PACKET_MESSAGE_TYPE_TO_INT(t) (PacketMessageType)t
#define RPC_PACKET_TYPE_TO_INT(t) (PacketType)t

#define    _REQUEST_LINK_        0xff
#define    _CONFIRM_LINK_        0xfe
#define    _CALL_METHOD_         0xfd
#define    _RETURN_FROM_CALL_    0xfc
#define    _REQUEST_DISCONNECT_  0xfb
#define    _CONFIRM_DISCONNECT_  0xfa
#define    _VOID_                0xf9
#define    _SIGNAL_FORWARD_      0xf8
#define    _SIGNAL_CONNECT_      0xf7
#define    _SIGNAL_CONF_         0xf6
#define    _SIGNAL_CONF_CONN_    0xf5
#define    _REQUEST_             0xa0
#define    _CONFIRMATION_        0xa1

#define    _SYNCHRONOUS_         0xb0
#define    _ASYNCHRONOUS_        0xb1

#define    _RPC_BINDING_         0xe0
#define    _SIG_SLOT_BINDING_    0xe1

#define RPC_BEGIN_FORWARDING_DECLARATION
#define RPC_END_FORWARDING_DECLARATION
#define RPC_BEGIN_FORWARD_BODY
#define RPC_END_FORWARD_BODY

#define ASSIGN 0
#define RVALUE 1
}







#endif // RPC_GLOBAL_H
