# #####################################################################
# MultiFusion main module makefile. Serkov Alexander, 2007
# #####################################################################
LIBS += -L./../libs \
    -lpaintWidget
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
DESTDIR = ./../../build/bin
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
