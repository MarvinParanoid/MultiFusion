TEMPLATE = lib
TARGET = SaveSVG
CONFIG += plugin
DEPENDPATH += . \
    ./../../pluginTool
INCLUDEPATH += . \
    ./../../pluginTool \

DESTDIR = ./../../bin/plugins/SaveSVG
QT += core \
    gui \
    xml \
    xmlpatterns \
    svg \
    opengl
HEADERS += SaveSVG.h SignalHolder.h
SOURCES += SaveSVG.cpp


