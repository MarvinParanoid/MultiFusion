TEMPLATE = lib
TARGET = Tutorials
CONFIG += plugin
DEPENDPATH += . \
    ./../../pluginTool
INCLUDEPATH += . \
    ./../../pluginTool \
    ./../../interfaces
LIBS += -L./../../pluginTool
DESTDIR = ./../../bin/plugins/Tutorials
QT += core \
    gui \
    xml \
    xmlpatterns \
    webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
lessThan(QT_MAJOR_VERSION, 5): QT += webkit

HEADERS += HelpWindow.h \
        Tutorials.h \
    Tutorial.h \
	Plugin.h \
    InterfacePlugin.h \
    ./../../pluginTool/SignalHolder.h
SOURCES += HelpWindow.cpp \
    Tutorials.cpp
FORMS = helpwindow.ui

TRANSLATIONS = Tutorials_ru.ts
