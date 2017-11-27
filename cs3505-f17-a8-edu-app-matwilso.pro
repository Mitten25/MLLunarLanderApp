#-------------------------------------------------
#
# Project created by QtCreator 2017-11-26T15:52:01
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cs3505-f17-a8-edu-app-matwilso
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INCLUDEPATH += $$PWD/Box2D/Box2D/


# This links the Box2D library for Linux machines
unix:!macx: LIBS += -L"$$PWD/Box2D/Box2D/Build/gmake/bin/Debug" -lBox2D

# Instructions for setting up on Linux:
# install premake5 (https://premake.github.io/download.html) and run this command in the Box2D/Box2D folder:
# premake5 --os=linux gmake premake5.lua
