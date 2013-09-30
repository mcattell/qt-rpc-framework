QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = serverguiexampleapp
TEMPLATE = app
INCLUDEPATH += \
            ../shared \
            ../rpcclient

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

LIBS += -L$$[QT_INSTALL_LIBS] -lrpcclient
