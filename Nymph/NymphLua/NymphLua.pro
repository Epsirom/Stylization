#-------------------------------------------------
#
# Project created by QtCreator 2014-07-30T16:28:12
#
#-------------------------------------------------

QT       -= gui

TARGET = NymphLua
TEMPLATE = lib
CONFIG += staticlib

SOURCES += nymphlua.cpp

HEADERS += nymphlua.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../NymphCore/release/ -lNymphCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../NymphCore/debug/ -lNymphCore
else:unix: LIBS += -L$$OUT_PWD/../NymphCore/ -lNymphCore

INCLUDEPATH += $$PWD/../NymphCore
DEPENDPATH += $$PWD/../NymphCore
