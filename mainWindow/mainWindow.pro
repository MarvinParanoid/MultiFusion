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
    ./paintWidget
DESTDIR = ./../bin
SOURCES += \
    ./../pluginTool/Plugin.cpp \
    Main.cpp \
    MainWindow.cpp
HEADERS += \
    ./../pluginTool/Plugin.h \
    MainWindowInterface.h \
    GObject.h \
    ./../pluginTool/PluginDialog.h \
    MainWindow.h
TRANSLATIONS += main_ru.ts
QT += opengl \
    xml \
    xmlpatterns
RESOURCES = main.qrc
