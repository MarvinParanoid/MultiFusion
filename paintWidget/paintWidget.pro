TEMPLATE = lib
CONFIG += static
INCLUDEPATH += . \
    ./../interfaces \
    ./../pluginTool
TARGET = PaintWidget
DESTDIR = ./../libs
DEPENDPATH += . \
    ./../pluginTool \
    ./../interfaces
SOURCES += UndoStructure.cpp \
    GLayer.cpp \
    ColorBox.cpp \
    GContainer.cpp \
    GObject.cpp \
    GSelectionRect.cpp \
    GVectorFigure.cpp \
    GuiUtils.cpp \
    PWE.cpp \
    PaintWidget.cpp \
    RPW.cpp
HEADERS += UndoStructure.h \
    GLayer.h \
    ColorBox.h \
    GContainer.h \
    GFramesProperties.h \
    GObject.h \
    GPropertiesObj.h \
    GSelectionRect.h \
    GVectorFigure.h \
    GuiUtils.h \
    PWE.h \
    PaintWidget.h \
    RPW.h \
    Utils.h \
    InterfacePlugin.h \
    plugin.h
QT += opengl \
    gui \
    xml \
    xmlpatterns
