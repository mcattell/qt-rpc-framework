QT += core network
QT -= gui

TARGET = rpcserver
TEMPLATE = lib
CONFIG += console

HEADERS +=\
            $$[QT_INSTALL_HEADERS] \
            ../rpcclient \
            ../shared


DEFINES += RPCSERVER_LIBRARY
#DEFINES += DEBUG_RPC_SERVER

SOURCES += \
        rpcserver.cpp \
        daemon.cpp \
        server_handler.cpp \
        server_link_handler.cpp \
        server_methodcall_handler.cpp \
        server_signal_handler.cpp \
        server_packetfactory.cpp \
        server_controller.cpp \
        server_link.cpp \
        server_metaservice.cpp \
        server_invokation.cpp \
        server_rpc_protocol.cpp


HEADERS += \
        rpcserver.h\
        rpcserver_global.h \
        rpcserver_p.h \
        daemon.h \
        server_handler.h \
        server_packetfactory.h \
        server_controller.h \
        server_link.h \
        server_metaservice.h \
        server_invokation.h \
        server_rpc_protocol.h

include(../shared/shared.pri)

target.path = $$[QT_INSTALL_LIBS]
INSTALLS += target

