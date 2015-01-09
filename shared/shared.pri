# Shared dependencies for server and client libraries

DEPENDPATH += $$PWD
INCLUDEPATH += $PWD

SOURCES += ../shared/controller.cpp \
           ../shared/link.cpp \
           ../shared/metaservice.cpp \
           ../shared/packet.cpp \
           ../shared/handler.cpp

HEADERS += ../shared/controller.h \
           ../shared/link.h \
           ../shared/metaservice.h \
           ../shared/packet.h \
           ../shared/handler.h
