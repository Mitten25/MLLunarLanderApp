#ifndef _ENV_H_

#include "mainwindow.h"
#include <QApplication>
#include "Box2D/Box2D.h"
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "QtMath"
#include "tuple"
#include "vector"

class Env
{
public:
    virtual std::tuple<std::vector<float>, float, bool> reset();
    virtual std::tuple<std::vector<float>, float, bool> step(std::vector<float>);
    virtual void render();
    //TODO: close() maybe
};


#endif
#define _ENV_H_
