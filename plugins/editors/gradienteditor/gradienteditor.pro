######################################################################
#	GradientEditor MultiFusion build script. Kukushkin Alexey, 2009
######################################################################

TEMPLATE = lib
TARGET = GradientEditor
CONFIG += plugin
SOURCES += GradientEditor.cpp \
	GradientStopsEditor.cpp \
	GradientRangeEditor.cpp
HEADERS += GradientEditor.h \
	GradientStopsEditor.h \
	GradientRangeEditor.h \
	Plugin.h \
        ./../../../pluginTool/SignalHolder.h
DEPENDPATH += . \
	./../../../pluginTool
INCLUDEPATH += . \
        ./../../../pluginTool
LIBS += -L./../../../pluginTool
DESTDIR = ./../../../bin/plugins/GradientEditor
FORMS += gradientform.ui
QT += xml \
    xmlpatterns \
    widgets
