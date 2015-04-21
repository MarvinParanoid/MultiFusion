######################################################################
#       Scale plugin. Demidov Alexey, 2014
######################################################################
TEMPLATE = lib
TARGET = Scale
CONFIG += plugin
DEPENDPATH += ./../../../pluginTool
INCLUDEPATH += ./../../../pluginTool
QT += xml \
    xmlpatterns \
    opengl
DESTDIR = ./../../../bin/plugins/Scale
SOURCES += Scale.cpp
HEADERS += Scale.h \
    Plugin.h \
    SignalHolder.h \
    QDRuler.h
