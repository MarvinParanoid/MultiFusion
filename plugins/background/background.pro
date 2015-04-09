# #####################################################################
# Background MultiFusion build script. Kukushkin Alexey, 2009
# #####################################################################
TEMPLATE = lib
TARGET = Background
CONFIG += plugin
DEPENDPATH += . \
    ./../../pluginTool
INCLUDEPATH += . \
    ./../../pluginTool

DESTDIR = ./../../bin/plugins/Background
HEADERS += Background.h \
    Plugin.h \
    SignalHolder.h
SOURCES += Background.cpp
QT += xml \
    xmlpatterns \
	opengl 
