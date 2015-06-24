TEMPLATE = lib
TARGET = ClippingPath
CONFIG += plugin
DEPENDPATH += . \
    ./../../../pluginTool \
    ./../../../interfaces \
    ./../../../paintWidget
INCLUDEPATH += . \
    ./../../../pluginTool \
    ./../../../interfaces \
    ./../../../paintWidget
QT += opengl \
    xml \
    xmlpatterns
LIBS += -L./../../../pluginTool
DESTDIR = ./../../../bin/plugins/ClippingPath

SOURCES += ClippingPath.cpp \
    ../../../PaintWidget/GContainer.cpp \
    ../../../PaintWidget/GVectorFigure.cpp
RESOURCES += ClippingPath.qrc
HEADERS += ClippingPath.h \
    ../../../pluginTool/InterfacePlugin.h \
    ../../../pluginTool/SignalHolder.h \
    ../../../pluginTool/Plugin.h \
    ../../../interfaces/FigureToolInterface.h \
    ../../../interfaces/MainWindowInterface.h \
    ../../../interfaces/PaintWidgetInterface.h \
    ../../../Interfaces/RPWInterface.h \
    ../../../Interfaces/GSRInterface.h \
    ../../../PaintWidget/GContainer.h \
    ../../../PaintWidget/GObject.h \
    ../../../PaintWidget/GVectorFigure.h \
    ../../../Interfaces/GContainerInterface.h \
