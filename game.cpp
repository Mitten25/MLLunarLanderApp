#include "mainwindow.h"
#include <QApplication>
#include "Box2D/Box2D.h"
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "QtMath"
#include "tuple"
#include "envs/env.h"
#include "envs/lunarlander.h"
#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

#undef main
//static const float SCALE = 30.f;

/** Bots */
std::vector<float> xBasedBot(std::vector<float> obs);

int game()
{
    LunarLander env(false);

    // discrete
    int actionCount = 4;
    int observationSpace = 8;

    EnvData envData = env.reset();

    float episodeReward = 0;

    // loop forever running episodes of lunar lander
    int count = 0;
    // take first observation
    std::vector<float> observation;
    std::vector<float> s(1, 0.0f);
    envData = env.step(s);
    observation = envData.observation;
    while (1) {
        count++;
        //std::vector<float> action;
        //action.push_back(std::floor(dis(gen)));
        //std::cout << action.front() << std::endl;

        // update the sf::RenderWindow with the new location of stuff (redraw basically)

        env.render();
        envData = env.step(xBasedBot(observation));
        observation = envData.observation;
        episodeReward += envData.reward;

        //std::cout << "obs: ";
        //for (std::vector<float>::const_iterator i = envData.observation.begin(); i != envData.observation.end(); ++i)
        //    std::cout << *i << ' ';
        //std::cout << std::endl;

        // lander has crashed or landed successfully or timed out
        if (envData.done) {
            envData = env.reset();
            std::cout << "done, ep reward = " << episodeReward << std::endl;
            episodeReward = 0;
            //std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}

std::vector<float> xBasedBot(std::vector<float> obs){
    //observation values: x, y, xvel, yvel, angle, anglevel, leftleg, rightleg
    //(noop, left engine, main engine, right engine)
    // -anglevel : right , +anglevel : left
    float arr[] = {1.0f};
    if (obs[2] < -0.05){
        arr[0] = 3.0f;
    }
    else if (obs[2] > 0.05){
        arr[0] = 1.0f;
    }
    else if (obs[3] < -0.25){
        arr[0] = 2.0f;
    }
    else{
        arr[0] = 0.0f;
    }
    std::vector<float> rtn (arr, arr + sizeof(arr) / sizeof(arr[0]) );
    return rtn;

}
