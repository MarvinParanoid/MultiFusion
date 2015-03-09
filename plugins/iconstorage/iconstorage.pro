######################################################################
#	IconStorage SACS2 build script. Serkov Alexander, 2007
######################################################################

TEMPLATE = lib
TARGET = iconstorage
CONFIG += plugin
DEPENDPATH += . ./../../pluginsdk ./
INCLUDEPATH += . ./../../pluginsdk ./../../interfaces
LIBS += -L./../../pluginsdk -lplugin
DESTDIR = ./../../bin/plugins

HEADERS += iconstorage.h
SOURCES += iconstorage.cpp
RESOURCES += iconstorage.qrc
