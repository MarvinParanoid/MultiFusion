TEMPLATE = lib
TARGET = SaveSVG
CONFIG += plugin
DEPENDPATH += . \
    ./../../pluginTool
INCLUDEPATH += . \
    ./../../pluginTool \
LIBS += -L./../../pluginTool
DESTDIR = ./../../bin/plugins/SaveSVG
QT += core \
    gui \
    xml \
    xmlpatterns \
    svg \
    opengl
HEADERS += savesvg.h SignalHolder.h
SOURCES += savesvg.cpp
