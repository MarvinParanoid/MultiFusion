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
HEADERS += helpwindow.h \
	tutorials.h \
    tutorial.h \
	Plugin.h \
    InterfacePlugin.h \
    SignalHolder.h
SOURCES += helpwindow.cpp \
    tutorials.cpp
FORMS = helpwindow.ui
