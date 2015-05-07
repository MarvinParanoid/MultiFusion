# #####################################################################
# PluginSDK project for SACS2, Serkov Alexander, 2007
# #####################################################################
TEMPLATE = lib
CONFIG += static
TARGET = Plugin
DEPENDPATH += .
INCLUDEPATH += .
DESTDIR = ./../libs

# Input
HEADERS += InterfacePlugin.h PluginDialog.h SignalHolder.h \
    Plugin.h
SOURCES += \
    Plugin.cpp
TRANSLATIONS += plugin_ru.ts
QT += core \
	opengl \
	xml \
    xmlpatterns
