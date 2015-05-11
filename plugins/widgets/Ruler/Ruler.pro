######################################################################
#       Ruler plugin. Demidov Alexey, 2014
######################################################################
TEMPLATE = lib
TARGET = Ruler
CONFIG += plugin
DEPENDPATH += ./../../../pluginTool
INCLUDEPATH += ./../../../pluginTool
QT += xml \
    xmlpatterns \
    widgets
DESTDIR = ./../../../bin/plugins/Ruler
SOURCES += Ruler.cpp \
    WayLine.cpp \
    QDRuler.cpp
HEADERS += Ruler.h \
    Plugin.h \
    ./../../../pluginTool/SignalHolder.h \
    QDRuler.h \
    WayLine.h

