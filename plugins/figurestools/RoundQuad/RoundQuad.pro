######################################################################
#	RoundQuad tool SACS2 build script. Serkov Alexander, 2007
######################################################################

TEMPLATE = lib
TARGET = RoundQuad
CONFIG += plugin
DEPENDPATH += ./../../pluginTool
INCLUDEPATH += ./../../../interfaces \
     ./../../pluginTool
DESTDIR = ./../../../bin/plugins/RoundQuad

HEADERS += RoundQuad.h \
    Plugin.h \
    FigureToolInterface.h \
    ./../../pluginTool/SignalHolder.h
SOURCES += RoundQuad.cpp
RESOURCES += roundquad.qrc
QT += core \
    gui \
    xml \
    xmlpatterns
