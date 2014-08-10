#-------------------------------------------------
#
# Project created by QtCreator 2014-08-01T15:42:03
#
#-------------------------------------------------

VERSION = 0.0.2

CONFIG += use_lua

include(../paths.pri)

QT       += core gui

TARGET = NymphLua
TEMPLATE = lib

DEFINES += NYMPHLUA_LIBRARY

SOURCES += nymphlua.cpp \
    nymphlua_ext.cpp

HEADERS += nymphlua.h\
        nymphlua_global.h \
    nymphlua_ext.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../NymphCore/release/ -lNymphCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../NymphCore/debug/ -lNymphCore
else:unix: LIBS += -L$$OUT_PWD/../NymphCore/ -lNymphCore

INCLUDEPATH += $$PWD/../NymphCore
DEPENDPATH += $$PWD/../NymphCore
