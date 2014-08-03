#-------------------------------------------------
#
# Project created by QtCreator 2014-08-01T15:42:32
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
    nymphsingleviewer.cpp \
    nymphrunner.cpp

HEADERS  += mainwindow.h \
    nympheditor.h \
    nymphgui_global.h \
    resultwindow.h \
    nymphviewer.h \
    nymphsingleviewer.h \
    nymphrunner.h

FORMS    += mainwindow.ui \
    resultwindow.ui \
    nymphviewer.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../NymphLua/release/ -lNymphLua
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../NymphLua/debug/ -lNymphLua
else:unix: LIBS += -L$$OUT_PWD/../NymphLua/ -lNymphLua

INCLUDEPATH += $$PWD/../NymphLua
DEPENDPATH += $$PWD/../NymphLua

RESOURCES += \
    mainwindow.qrc

