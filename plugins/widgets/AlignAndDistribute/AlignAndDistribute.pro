# #####################################################################
# AlignAndDistribute MultiFusion build script. Mangush Vadim, 2015
# #####################################################################
TEMPLATE = lib
TARGET = AlignAndDistribute
CONFIG += plugin
DEPENDPATH += . \
	./../../../pluginTool \
	./../../../interfaces
INCLUDEPATH += . \
	./../../../pluginTool \
	./../../../interfaces
LIBS += -L./../../../pluginTool
QT += xml \
	qt3support \
	xmlpatterns
DESTDIR = ./../../../bin/plugins/AlignAndDistribute
SOURCES += AlignAndDistribute.cpp
HEADERS += AlignAndDistribute.h \
	Plugin.h \
	InterfacePlugin.h \
        SignalHolder.h
RESOURCES += AlignAndDistribute.qrc
FORMS += AlignAndDistribute.ui
