# #####################################################################
# ToolBox SACS2 build script. Serkov Alexander, 2007
# #####################################################################
DEPENDPATH += . \
    ./../../../pluginTool
INCLUDEPATH += . \
    ./../../../pluginTool
LIBS += -L./../../../pluginTool
TEMPLATE = lib
CONFIG += plugin
TARGET = UndoRedo
DESTDIR = ./../../../bin/plugins/UndoRedo
SOURCES += UndoRedo.cpp
HEADERS += UndoRedo.h \
    Plugin.h \
    ./../../../pluginTool/SignalHolder.h
QT += opengl \
	xml \
    xmlpatterns
