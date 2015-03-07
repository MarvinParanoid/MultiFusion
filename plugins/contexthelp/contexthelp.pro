TEMPLATE = lib
TARGET = ContextHelp
CONFIG += plugin
DEPENDPATH += ./../../pluginTool
INCLUDEPATH += ./../../pluginTool
LIBS += -L./../../pluginTool
DESTDIR = ./../../bin/plugins/ContextHelp
QT += core \
    gui \
    xml \
    xmlpatterns \
    webkit
HEADERS += helpwindow.h \
    contexthelp.h \
    Plugin.h \
    SignalHolder.h
SOURCES += helpwindow.cpp \
    contexthelp.cpp
FORMS = helpwindow.ui
