QT       += core network
QT       -= gui

TARGET = rpcclient
TEMPLATE = lib
CONFIG   += console

DEFINES += RPCCLIENT_LIBRARY
#DEFINES += DEBUG_RPCCLIENT

INCLUDEPATH +=\
            $$[QT_INSTALL_HEADERS] \
            $$PWD/../shared

DEPENDPATH += $$PWD/../shared

SOURCES += \
        rpcclient.cpp \
        client_handler.cpp \
        client_link_handler.cpp \
        client_signal_handler.cpp \
        client_method_handler.cpp \
        client_packetfactory.cpp \
        client_controller.cpp \
        client_link.cpp \
        client_metaservice.cpp \
        client_invokation.cpp \
        client_rpc_protocol.cpp \
        callinterface.cpp

HEADERS += \
	packet.h \
        link.h \
        packet.h \
	rpcclient.h \
        rpcclient_global.h \
        rpcclient_p.h \
        controller.h \
        rpc_global.h \
        handler.h \
        client_handler.h \
        metaservice.h \
        client_packetfactory.h \
        client_controller.h \
        client_link.h \
        client_metaservice.h \
        invokation.h \
        client_invokation.h \
        client_rpc_protocol.h \
        callinterface.h

include(../shared/shared.pri)
target.path = $$[QT_INSTALL_LIBS]
INSTALLS += target others


