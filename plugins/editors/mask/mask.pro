TEMPLATE = lib
TARGET = Mask
CONFIG += plugin
DEPENDPATH += . \
        ./../../../pluginTool \
        ./../../../interfaces
INCLUDEPATH += . \
        ./../../../pluginTool \
        ./../../../interfaces
HEADERS += Mask.h \
        Plugin.h \
        ./../../../pluginTool/SignalHolder.h \
        ../../../../interfaces/MainWindowInterface.h \
        ../../../../interfaces/PaintWidgetInterface.h \
        ../../../../Interfaces/RPWInterface.h \
        ../../../../Interfaces/GSRInterface.h \
        ../../../../Interfaces/GradientEditorInterface.h
SOURCES += Mask.cpp
LIBS += -L./../../../pluginTool
DESTDIR = ./../../../bin/plugins/Mask
QT += opengl \
    xml \
    xmlpatterns
