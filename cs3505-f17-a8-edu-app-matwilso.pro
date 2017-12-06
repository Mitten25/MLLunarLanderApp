#-------------------------------------------------
#
# Project created by QtCreator 2017-11-26T15:52:01
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets concurrent

TARGET = cs3505-f17-a8-edu-app-matwilso
TEMPLATE = app


SOURCES +=\
        mainwindow.cpp \
    game.cpp \
    main.cpp \
    envs/lunarlander.cpp \
    screenwelcome.cpp \
    screenintro.cpp \
    screenreward.cpp \
    screenobservation.cpp \
    screentrials.cpp \
    screenlander.cpp
HEADERS  += mainwindow.h \
    lunarlander.h \
    screenwelcome.h \
    screenintro.h \
    screenreward.h \
    screenobservation.h \
    screentrials.h \
    screenlander.h \
    envs/env.h \
    envs/lunarlander.h
FORMS    += mainwindow.ui \


# reinforcement learning source (need to test on windows)
unix: SOURCES+= rl/reinforce.cpp
unix: HEADERS+= rl/reinforce.h
unix:INCLUDEPATH += $$PWD/include/armadillo-8.300.1/include/
unix:INCLUDEPATH += $$PWD/include/OpenBLAS-0.2.20/include/
# ARMADILLO
unix: LIBS += -L"$$PWD/include/armadillo-8.300.1/"
unix: LIBS += -larmadillo
# OpenBLAS
unix: LIBS += -L"$$PWD/include/OpenBLAS-0.2.20/"
unix: LIBS += -lopenblas

INCLUDEPATH += $$PWD/include/Box2D/Box2D/
INCLUDEPATH += $$PWD/include/SFML/include/
#DEPENDPATH += $$PWD/include/SFML/include/


# Windows
# SFML
win32-msvc*{
    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/include/SFML/lib/release/ -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-window -lsfml-system
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/include/SFML/lib/debug/ -lsfml-audio-d -lsfml-graphics-d -lsfml-network-d -lsfml-window-d -lsfml-system-d
}
win32-g++: LIBS += -L$$PWD/include/SFML/lib/ -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-window -lsfml-system

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
unix: LIBS += -lsfml-graphics -lsfml-network -lsfml-window -lsfml-system

# Box2d
unix: LIBS += -L"$$PWD/include/Box2D/Box2D/Build/gmake/bin/Debug" -lBox2D
#unix:!macx: LIBS += -lsfml-graphics -lsfml-window -lsfml-system




#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/include/Box2D/Box2D/Build/gmake/bin/release/ -lBox2D
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/include/Box2D/Box2D/Build/gmake/bin/debug/ -lBox2D

#INCLUDEPATH += $$PWD/include/Box2D/Box2D/Build/gmake/bin/Debug
#DEPENDPATH += $$PWD/include/Box2D/Box2D/Build/gmake/bin/Debug

include(include/Box2D/Box2D/Box2D/Box2D.pro)
