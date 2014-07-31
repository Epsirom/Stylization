#-------------------------------------------------
#
# Project created by QtCreator 2014-07-30T16:28:25
#
#-------------------------------------------------

QT       -= gui

TARGET = NymphCore
TEMPLATE = lib

DEFINES += NYMPHCORE_LIBRARY

SOURCES += nymphcore.cpp

HEADERS += nymphcore.h\
        nymphcore_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
