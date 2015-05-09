# #####################################################################
# PenEditor MultiFusion build script. Kukushkin Alexey, 2009
# #####################################################################
TEMPLATE = lib
TARGET = PenEditor
CONFIG += plugin
DEPENDPATH += . \
    ./../../../pluginTool
INCLUDEPATH += . \
    ./../../../pluginTool
LIBS += -L./../../../pluginTool
DESTDIR = ./../../../bin/plugins/PenEditor
HEADERS += PenEditor.h \
    Plugin.h \
    ./../../../pluginTool/SignalHolder.h
SOURCES += PenEditor.cpp
FORMS += peneditor.ui
QT += xml \
    xmlpatterns \
    widgets
