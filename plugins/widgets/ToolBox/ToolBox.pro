######################################################################
#       ToolBox SACS2 build script. Serkov Alexander, 2007
######################################################################

DEPENDPATH += ./../../../pluginTool
INCLUDEPATH += ./../../../pluginTool
LIBS += -L./../../../pluginTool

TEMPLATE = lib
CONFIG += plugin

TARGET = ToolBox
DESTDIR = ./../../../bin/plugins/ToolBox
SOURCES += ToolBox.cpp
HEADERS += ToolBox.h \
	Plugin.h \
        ./../../../pluginTool/SignalHolder.h
RESOURCES += ToolBox.qrc
TRANSLATIONS += toolbox_ru.ts
QT += opengl \
	xml \
    xmlpatterns
