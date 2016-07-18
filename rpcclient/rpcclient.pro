QT       += core network
QT       -= gui

TARGET = rpcclient
TEMPLATE = lib
CONFIG   += console

INCLUDEPATH += "../shared"

DEFINES += RPCCLIENT_LIBRARY
#DEFINES += DEBUG_RPCCLIENT


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
	rpcclient.h \
        rpcclient_global.h \
        rpcclient_p.h \
        client_handler.h \
        client_packetfactory.h \
        client_controller.h \
        client_link.h \
        client_metaservice.h \
        client_invokation.h \
        client_rpc_protocol.h \
        callinterface.h

include(../shared/shared.pri)

target.path = $$[QT_INSTALL_LIBS]
INSTALLS += target


