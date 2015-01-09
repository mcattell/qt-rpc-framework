QT       += core network

QT       -= gui

TARGET = testrpcserver
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
INCLUDEPATH +=\
    $$PWD \
    $$[QT_INSTALL_HEADERS] \
        ../rpcserver \
        ../rpcclient \
        ../shared \
        ../testrpcclient


DEPENDPATH += \
            $$PWD \
            ../shared \
            ../testrpcclient

LIBS += -L$$[QT_INSTALL_LIBS] -lrpcserver

SOURCES += \
           main.cpp


HEADERS += \
           roctestclass.h

include(../testrpcclient/testrpcclient.pri)
