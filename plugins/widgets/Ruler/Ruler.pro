######################################################################
#       Scale plugin. Demidov Alexey, 2014
######################################################################
TEMPLATE = lib
TARGET = Ruler
CONFIG += plugin
DEPENDPATH += ./../../../pluginTool
INCLUDEPATH += ./../../../pluginTool
QT += xml \
    xmlpatterns \
    opengl
DESTDIR = ./../../../bin/plugins/Ruler
SOURCES += Ruler.cpp
HEADERS += Ruler.h \
    Plugin.h \
    SignalHolder.h \
    QDRuler.h
TRANSLATION = ruler_ru.ts   \
                ruler_en.ts
