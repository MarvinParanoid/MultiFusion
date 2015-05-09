######################################################################
#	BrushEditor SACS2 build script. Serkov Alexander, 2007
######################################################################

TEMPLATE = lib
TARGET = BrushEditor
CONFIG += plugin
DEPENDPATH += . ./../../../pluginTool
INCLUDEPATH += . ./../../../pluginTool
LIBS += -L./../../../pluginTool
DESTDIR = ./../../../bin/plugins/BrushEditor

HEADERS += BrushEditor.h \
	Plugin.h \
	TexturesList.h \
        ./../../../PluginTool/SignalHolder.h
SOURCES += BrushEditor.cpp
FORMS += brusheditor.ui \
        texturesform.ui
QT += xml \
    xmlpatterns \
    widgets
