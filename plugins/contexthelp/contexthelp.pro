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
    webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
lessThan(QT_MAJOR_VERSION, 5): QT += webkit

HEADERS += HelpWindow.h \
    ContextHelp.h \
    Plugin.h \
    ./../../PluginTool/SignalHolder.h
SOURCES += HelpWindow.cpp \
    ContextHelp.cpp
FORMS = helpwindow.ui

TRANSLATIONS = ContextHelp_ru.ts
