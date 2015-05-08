# #####################################################################
# AlignAndDistribute MultiFusion build script. Mangush Vadim, 2015
# #####################################################################
TEMPLATE = lib
TARGET = AlignAndDistribute
CONFIG += plugin
DEPENDPATH += . \
    ./../../../pluginTool
INCLUDEPATH += . \
    ./../../../pluginTool
LIBS += -L./../../../pluginTool
QT += xml \
    xmlpatterns \
    widgets
DESTDIR = ./../../../bin/plugins/AlignAndDistribute
SOURCES += AlignAndDistribute.cpp
HEADERS += AlignAndDistribute.h \
    Plugin.h \
    InterfacePlugin.h \
    ./../../../pluginTool/SignalHolder.h
TRANSLATIONS = AlignAndDistribute_ru.ts
RESOURCES += AlignAndDistribute.qrc
FORMS += AlignAndDistribute.ui
