######################################################################
#	Rectangle tool MultiFusion build script. Kukushkin Alexey, 2009
######################################################################

TEMPLATE = lib
TARGET = Quad
CONFIG += plugin
DEPENDPATH += . \
    ./../../../pluginTool
INCLUDEPATH += . \
    ./../../../pluginTool \
    ./../../../interfaces 
LIBS += -L./../../../pluginTool
DESTDIR = ./../../../bin/plugins/Quad
HEADERS += Quad.h \
	Plugin.h \
	FigureToolInterface.h \
	InterfacePlugin.h \
        ./../../../pluginTool/SignalHolder.h
SOURCES += Quad.cpp
RESOURCES += Quad.qrc
QT += xml \
    xmlpatterns \
    widgets
