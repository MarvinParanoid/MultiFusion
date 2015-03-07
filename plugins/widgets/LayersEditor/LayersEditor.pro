# #####################################################################
# LayersEditor MultiFusion build script. Kukushkin Alexey, 2009
# #####################################################################
DEPENDPATH += . \
    ./../../../pluginTool
INCLUDEPATH += . \
    ./../../../pluginTool
LIBS += -L./../../../pluginTool
TEMPLATE = lib
CONFIG += plugin
TARGET = LayersEditor
DESTDIR = ./../../../bin/plugins/LayersEditor
SOURCES += layerseditor.cpp
HEADERS += LayersEditor.h \
    Plugin.h \
	SignalHolder.h
RESOURCES += LayersEditor.qrc
TRANSLATIONS = layer_ru.ts
QT += xml \
    xmlpatterns
