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
HEADERS += HelpWindow.h \
    ContextHelp.h \
    Plugin.h \
    SignalHolder.h
SOURCES += HelpWindow.cpp \
    ContextHelp.cpp
FORMS = helpwindow.ui
