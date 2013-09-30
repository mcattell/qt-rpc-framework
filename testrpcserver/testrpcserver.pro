QT       += core network

QT       -= gui

TARGET = testrpcserver
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
INCLUDEPATH +=\
    $$[QT_INSTALL_HEADERS] \
        ../rpcserver \
        ../shared \
        ../testrpcclient


DEPENDPATH += \
            ../shared \
            ../testrpcclient

LIBS += -L$$[QT_INSTALL_LIBS] -lrpcserver

SOURCES += \
           main.cpp \
           roctestclass.cpp \
           clienttestclass.cpp

HEADERS += \
           roctestclass.h \
           clienttestclass.h


