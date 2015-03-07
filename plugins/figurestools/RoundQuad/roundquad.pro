######################################################################
#	RoundQuad tool SACS2 build script. Serkov Alexander, 2007
######################################################################

TEMPLATE = lib
TARGET = RoundQuad
CONFIG += plugin
DEPENDPATH += . ./../../../pluginsdk ./../../pluginTool
INCLUDEPATH += . ./../../../pluginsdk ./../../../interfaces ./../../pluginTool
LIBS += -L./../../../pluginsdk
DESTDIR = ./../../../bin/plugins/RoundQuad

HEADERS += roundquad.h Plugin.h figuretoolinterface.h SignalHolder.h
SOURCES += roundquad.cpp
RESOURCES += roundquad.qrc
QT += core \
    gui \
    xml \
    xmlpatterns
