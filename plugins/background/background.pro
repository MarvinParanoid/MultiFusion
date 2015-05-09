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
LIBS += -L./../../libs
DESTDIR = ./../../bin/plugins/Background
HEADERS += Background.h \
    ./../../pluginTool/Plugin.h \
    ./../../pluginTool/SignalHolder.h
SOURCES += Background.cpp
QT += xml \
    xmlpatterns \
	opengl 
