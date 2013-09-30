QT       += network
QT       -= gui

TARGET = rpcserver
TEMPLATE = lib
INCLUDEPATH +=\
            $$[QT_INSTALL_HEADERS] \
            ../shared

DEPENDPATH += $$INCLUDEPATH
DEFINES += RPCSERVER_LIBRARY
#DEFINES += DEBUG_RPC_SERVER

SOURCES += \
        rpcserver.cpp \
        daemon.cpp \
        packet.cpp \
        handler.cpp \
        server_handler.cpp \
        controller.cpp \
        server_link_handler.cpp \
        server_methodcall_handler.cpp \
        server_signal_handler.cpp \
        metaservice.cpp \
        server_packetfactory.cpp \
        server_controller.cpp \
        server_link.cpp \
        server_metaservice.cpp \
        server_invokation.cpp \
        link.cpp \
        server_rpc_protocol.cpp

HEADERS += \
        rpcserver.h\
        rpcserver_global.h \
        rpcserver_p.h \
        daemon.h \
        packet.h \
        controller.h \
        link.h \
        rpc_global.h \     
        handler.h \
        server_handler.h \
        protocol.h \
        metaservice.h \
        server_packetfactory.h \
        server_controller.h \
        server_link.h \
        server_metaservice.h \
        invokation.h \
        server_invokation.h \
        server_rpc_protocol.h


target.path = $$[QT_INSTALL_LIBS]
INSTALLS += target

