# #####################################################################
# PenBox MultiFusion build script. Kukushkin Alexey, 2009
# #####################################################################
DEPENDPATH += . \
    ./../../../pluginTool
INCLUDEPATH += . \
    ./../../../pluginTool \
LIBS += -L./../../../pluginTool
TEMPLATE = lib
CONFIG += plugin
TARGET = PenBox
DESTDIR = ./../../../bin/plugins/PenBox
SOURCES += PenBox.cpp
HEADERS += PenBox.h \
    Plugin.h \
    ./../../../pluginTool/SignalHolder.h
QT += xml \
    xmlpatterns \
    widgets
