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
    webkit
HEADERS += HelpWindow.h \
        Tutorials.h \
    Tutorial.h \
	Plugin.h \
    InterfacePlugin.h \
    SignalHolder.h
SOURCES += HelpWindow.cpp \
    Tutorials.cpp
FORMS = helpwindow.ui
