######################################################################
#	Spline tool MultiFusion build script. Kukushkin Alexey, 2009
######################################################################

TEMPLATE = lib
TARGET = Spline
CONFIG += plugin
DEPENDPATH += . \
    ./../../../pluginTool
INCLUDEPATH += . \
    ./../../../pluginTool \
    ./../../../interfaces
QT += xml \
    xmlpatterns \
    widgets
LIBS += -L./../../../pluginTool
DESTDIR = ./../../../bin/plugins/Spline
HEADERS += Spline.h \
    Plugin.h \
    FigureToolInterface.h \
    InterfacePlugin.h \
    ./../../../pluginTool/SignalHolder.h
SOURCES += Spline.cpp
RESOURCES += Spline.qrc
