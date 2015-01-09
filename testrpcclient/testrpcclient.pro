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
-L/home/matthew/dev/build-qtrpcframework-Desktop_Qt_5_4_0_GCC_64bit-Debug/rpcclient -lrpcclient
 #   -L$$[QT_INSTALL_LIBS] -lrpcclient

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



LIBS += -L/home/matthew/dev/qt5/install/lib -lrpcclient

