#-------------------------------------------------
#
# Project created by QtCreator 2014-07-30T16:13:46
#
#-------------------------------------------------

include(../paths.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NymphGUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    nympheditor.cpp \
    nymphgui_global.cpp \
    resultwindow.cpp \
    nymphviewer.cpp \
    nymphsingleviewer.cpp

HEADERS  += mainwindow.h \
    nympheditor.h \
    nymphgui_global.h \
    resultwindow.h \
    nymphviewer.h \
    nymphsingleviewer.h

FORMS    += mainwindow.ui \
    resultwindow.ui \
    nymphviewer.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../NymphLua/release/ -lNymphLua
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../NymphLua/debug/ -lNymphLua
else:unix: LIBS += -L$$OUT_PWD/../NymphLua/ -lNymphLua

INCLUDEPATH += $$PWD/../NymphLua
DEPENDPATH += $$PWD/../NymphLua

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../NymphLua/release/libNymphLua.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../NymphLua/debug/libNymphLua.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../NymphLua/release/NymphLua.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../NymphLua/debug/NymphLua.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../NymphLua/libNymphLua.a

RESOURCES += \
    mainwindow.qrc
