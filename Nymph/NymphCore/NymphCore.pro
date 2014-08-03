#-------------------------------------------------
#
# Project created by QtCreator 2014-08-01T15:41:35
#
#-------------------------------------------------

CONFIG += use_opencv

include(../paths.pri)

QT       += core gui

TARGET = NymphCore
TEMPLATE = lib

DEFINES += NYMPHCORE_LIBRARY

SOURCES += nymphcore.cpp \
    nymphcvbase.cpp \
    nymphcore_export.cpp

HEADERS += nymphcore.h\
        nymphcore_global.h \
    nymphcvbase.h \
    nymphcore_export.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
