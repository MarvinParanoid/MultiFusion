# #####################################################################
# BrushBox MultiFusion build script. Kukushkin Alexey, 2009
# #####################################################################
DEPENDPATH += . \
    ./../../../pluginTool
INCLUDEPATH += . \
    ./../../../pluginTool
LIBS += -L./../../../pluginTool
TEMPLATE = lib
CONFIG += plugin
TARGET = BrushBox
DESTDIR = ./../../../bin/plugins/BrushBox
SOURCES += BrushBox.cpp
HEADERS += BrushBox.h \
    Plugin.h \
        ./../../../pluginTool/SignalHolder.h
QT += xml \
    xmlpatterns \
    widgets
