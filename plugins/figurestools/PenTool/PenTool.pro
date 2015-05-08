# #####################################################################
# Pen tool MultiFusion build script. Kukushkin Alexey, 2009
# #####################################################################
TEMPLATE = lib
TARGET = PenTool
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
DESTDIR = ./../../../bin/plugins/PenTool
HEADERS += PenTool.h \
    Plugin.h \
    FigureToolInterface.h \
    InterfacePlugin.h \
    ./../../../pluginTool/SignalHolder.h
SOURCES += PenTool.cpp
RESOURCES += PenTool.qrc
