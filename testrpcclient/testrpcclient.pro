QT       += core

QT       -= gui

TARGET = testrpcclient
CONFIG   += console
CONFIG   -= app_bundle
INCLUDEPATH += \
            ../rpcclient \
            ../shared


DEPENDPATH += $$INCLUDEPATH
TEMPLATE = app
LIBS +=\
    -L$$[QT_INSTALL_LIBS] -lrpcclient

SOURCES += \
            main.cpp \
            clienttestclass.cpp \
            testrpcclass.cpp \
            roctestclass.cpp \
            sourcetestobject.cpp \
            sourcetestclasstwo.cpp
HEADERS += \
            clienttestclass.h \
            testrpcclass.h \
            roctestclass.h \
            sourcetestobject.h \
            sourcetestclasstwo.h


