# #####################################################################
# Line tool MultiFusion build script. Kukushkin Alexey, 2009
# #####################################################################

TEMPLATE = lib
TARGET = Line
CONFIG += plugin
DEPENDPATH += . \
    ./../../../pluginTool \
    ./../../../interfaces
INCLUDEPATH += . \
    ./../../../pluginTool \
    ./../../../interfaces 
QT += xml \
    xmlpatterns \
    widgets
LIBS += -L./../../../pluginTool
DESTDIR = ./../../../bin/plugins/Line
HEADERS += Line.h \
    Plugin.h \
    FigureToolInterface.h \
    InterfacePlugin.h \
    ./../../../pluginTool/SignalHolder.h
SOURCES += Line.cpp
RESOURCES += Line.qrc
