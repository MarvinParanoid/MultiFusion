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
HEADERS += SaveSVG.h \
    ./../../pluginTool/SignalHolder.h
SOURCES += SaveSVG.cpp

TRANSLATIONS = SaveSVG_ru.ts
