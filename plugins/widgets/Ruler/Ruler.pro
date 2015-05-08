######################################################################
#       Ruler plugin. Demidov Alexey, 2014
######################################################################
TEMPLATE = lib
TARGET = Ruler
CONFIG += plugin
DEPENDPATH += ./../../../pluginTool
INCLUDEPATH += ./../../../pluginTool
QT += xml \
    xmlpatterns
DESTDIR = ./../../../bin/plugins/Ruler
SOURCES += Ruler.cpp \
    WayLine.cpp
HEADERS += Ruler.h \
    SignalHolder.h \
    QDRuler.h \
    WayLine.h
