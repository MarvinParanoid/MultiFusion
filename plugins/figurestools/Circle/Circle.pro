######################################################################
#	Circle tool MultiFusion build script. Kukushkin Alexey, 2009
######################################################################

TEMPLATE = lib
TARGET = Circle
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
DESTDIR = ./../../../bin/plugins/Circle
HEADERS += Circle.h \
	Plugin.h \
    FigureToolInterface.h \
    InterfacePlugin.h \
    ./../../../pluginTool/SignalHolder.h
SOURCES += Circle.cpp
RESOURCES += Circle.qrc
