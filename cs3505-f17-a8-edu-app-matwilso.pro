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

win32: LIBS += -L$$PWD/include/SFML/lib/ -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-window -lsfml-system

else:win32:CONFIG(release, debug|release): LIBS += -L$$PWD/include/SFML/lib/release/ -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-window -lsfml-system

else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/include/SFML/lib/debug/ -lsfml-audio-d -lsfml-graphics-d -lsfml-network-d -lsfml-window-d -lsfml-system-d
#Not sure if following line will work with other windows compilers without debug/release folders

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
unix: LIBS += -L"$$PWD/include/SFML/lib"
unix: LIBS += -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-window -lsfml-system

# Box2d
unix: LIBS += -L"$$PWD/include/Box2D/Box2D/Build/gmake/bin/Debug" -lBox2D
#unix:!macx: LIBS += -lsfml-graphics -lsfml-window -lsfml-system



#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/include/Box2D/Box2D/Build/gmake/bin/release/ -lBox2D
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/include/Box2D/Box2D/Build/gmake/bin/debug/ -lBox2D

#INCLUDEPATH += $$PWD/include/Box2D/Box2D/Build/gmake/bin/Debug
#DEPENDPATH += $$PWD/include/Box2D/Box2D/Build/gmake/bin/Debug

win32: include(include/Box2D/Box2D/Box2D/Box2D.pro)
