######################################################################
#	Polygon tool MultiFusion build script. Kukushkin Alexey, 2009
######################################################################

TEMPLATE = lib
TARGET = Polygon
CONFIG += plugin
DEPENDPATH += . \
    ./../../../pluginTool
INCLUDEPATH += . \
    ./../../../pluginTool \
    ./../../../interfaces 
QT += xml \
    xmlpatterns \
    widgets
	
LIBS += -L./../../../pluginTool/Polygon
DESTDIR = ./../../../bin/plugins/Polygon

HEADERS += Polygon.h \
    Plugin.h \
    FigureToolInterface.h \
    InterfacePlugin.h \
    ./../../../pluginTool/SignalHolder.h
SOURCES += Polygon.cpp
RESOURCES += Polygon.qrc
