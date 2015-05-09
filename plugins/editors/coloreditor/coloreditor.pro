# #####################################################################
# ColorEditor MultiFusion build script. Kukushkin Alexey, 2009
# #####################################################################
TEMPLATE = lib
TARGET = ColorEditor
CONFIG += plugin
DEPENDPATH += ./../../../pluginTool
INCLUDEPATH += ./../../../pluginTool
LIBS += -L./../../../pluginTool
DESTDIR = ./../../../bin/plugins/ColorEditor
HEADERS += ColorEditor.h \
    Plugin.h \
    ./../../../pluginTool/SignalHolder.h
SOURCES += ColorEditor.cpp
QT += xml \
    xmlpatterns \
    widgets
