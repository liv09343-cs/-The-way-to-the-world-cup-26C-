QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PenaltyShootout
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
        game.cpp \
        mainwindow.cpp \
        startwindow.cpp \
        teamselectwindow.cpp \
        worldcupmanager.cpp \
        resultwindow.cpp \
        storyselectwindow.cpp \
        storyscenewindow.cpp \
        matchresultwindow.cpp

HEADERS += \
        game.h \
        mainwindow.h \
        startwindow.h \
        teamselectwindow.h \
        worldcupmanager.h \
        resultwindow.h \
        storyselectwindow.h \
        storyscenewindow.h \
        matchresultwindow.h

FORMS += \
        mainwindow.ui \
        startwindow.ui \
        teamselectwindow.ui \
        resultwindow.ui \
        storyselectwindow.ui \
        storyscenewindow.ui \
        matchresultwindow.ui

RESOURCES += \
        resources.qrc

QMAKE_CXXFLAGS += -std=c++17