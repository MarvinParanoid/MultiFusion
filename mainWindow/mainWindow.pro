# #####################################################################
# MultiFusion main module makefile. Serkov Alexander, 2007
# #####################################################################
LIBS += -L./../libs \
    -lPaintWidget
TEMPLATE = app
TARGET = MultiFusion
DEPENDPATH += . \
    ./../pluginTool \
    ./../paintWidget \
    ./../interfaces
INCLUDEPATH += . \
    ./../interfaces \
    ./../pluginTool \
DESTDIR = ./../bin
SOURCES += \
    Plugin.cpp \
    Main.cpp \
    MainWindow.cpp
HEADERS += \
    Plugin.h \
    MainWindowInterface.h \
    GObject.h \
    PluginDialog.h \
    MainWindow.h
TRANSLATIONS += main_ru.ts
QT += opengl \
    xml \
    xmlpatterns
RESOURCES = main.qrc
