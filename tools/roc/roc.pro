QT       += core network
QT       -= gui

TARGET = roc
TEMPLATE = app
CONFIG   += console
CONFIG   -= app_bundle


INCLUDEPATH +=\
            $$[QT_INSTALL_HEADERS] \
            ../
system(rcc -binary sources.qrc -o sources.rcc)

DEPENDPATH += $$INCLUDEPATH

SOURCES += \
        main.cpp \
        projectgenerator.cpp \
        filehelper.cpp

HEADERS += \  
        projectgenerator.h \
        filehelper.h

target.path = $$[QT_INSTALL_BINS]

other.files = ../../rpcclient/callinterface.h ../../rpcclient/rpc_global.h
other.path = $$[QT_INSTALL_HEADERS]

RESOURCES += \
    sources.qrc

INSTALLS += target other
