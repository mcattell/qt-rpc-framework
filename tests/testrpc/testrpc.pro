QT       += core testlib

QT       -= gui

TARGET = tst_testrpc
CONFIG   += console
CONFIG   -= app_bundle
INCLUDEPATH += \
            $$[QT_INSTALL_HEADERS]\
            ../../rpcclient \
            ../../rpcserver \
            ../../shared \


DEPENDPATH += $$INCLUDEPATH
TEMPLATE = app
LIBS +=\
    -L$$[QT_INSTALL_LIBS] -lrpcclient

SOURCES += tst_testrpc.cpp \
           rpc_rpctestclass.cpp

HEADERS += rpctestclass.h

DEFINES += SRCDIR=\\\"$$PWD/\\\"




LIBS += -L/home/matthew/dev/qt5/install/lib -lrpcclient
LIBS += -L/home/matthew/dev/qt5/install/lib -lrpcclient
