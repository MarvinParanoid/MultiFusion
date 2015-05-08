######################################################################
#	Text tool MultiFusion build script. Kukushkin Alexey, 2009
######################################################################

TEMPLATE = lib
TARGET = Text
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
DESTDIR = ./../../../bin/plugins/Text

HEADERS += Text.h \
    Plugin.h \
    FigureToolInterface.h \
    InterfacePlugin.h \
    ./../../../pluginTool/SignalHolder.h
SOURCES += Text.cpp
RESOURCES += Text.qrc
