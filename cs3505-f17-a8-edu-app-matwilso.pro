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



INCLUDEPATH += $$PWD/include/Box2D/Box2D/
INCLUDEPATH += $$PWD/include/SFML/include/
#DEPENDPATH += $$PWD/include/SFML/include/


# Windows
# SFML
# TODO: set up SFML for windows
# this one might be similar to the linux sfml (see the instructions below, else see:
# https://github.com/SFML/SFML/wiki/Tutorial:-Compile-and-Link-SFML-with-Qt-Creator

# Box2D
# TODO: set up Box2D for windows

# this is used instead of unix:!macx:
# win32:


# Linux

# Instructions for setting up on Linux (ubuntu):
# SFML
# cd ./include/SFML
# cmake .
# make
#
#
# Box2D
# install premake5 (https://premake.github.io/download.html) and run this command in the Box2D/Box2D folder:
# ./path_to/premake5 --os=linux gmake premake5.lua


# SFML
unix:!macx: LIBS += -L"$$PWD/include/SFML/lib"
unix:!macx:LIBS += -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-window -lsfml-system

# Box2d
unix:!macx: LIBS += -L"$$PWD/include/Box2D/Box2D/Build/gmake/bin/Debug" -lBox2D
#unix:!macx: LIBS += -lsfml-graphics -lsfml-window -lsfml-system



