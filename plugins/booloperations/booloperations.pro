TEMPLATE = lib
TARGET = BoolOperations
CONFIG += plugin
DEPENDPATH += . \
    ./../../pluginTool
INCLUDEPATH += . \
    ./../../pluginTool
LIBS += -L./../../pluginTool \
    -L./../../libs -lPaintWidget
DESTDIR = ./../../bin/plugins/BoolOperations
QT += xml \
    xmlpatterns \
    opengl
HEADERS += BoolOperations.h \
    Plugin.h \
    InterfacePlugin.h \
    ./../../pluginTool/SignalHolder.h
SOURCES += BoolOperations.cpp
TRANSLATIONS = BoolOperations_ru.ts

FORMS += \
    booloperations.ui

RESOURCES += \
    booloperations.qrc
