#include "mainwindow.h"
#include <QApplication>
#include "Box2D/Box2D.h"
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "QtMath"
#include "tuple"
#include "env.h"
#include "lunarlander.h"
#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

#undef main
//static const float SCALE = 30.f;

/** Player input */
bool input_up = false;
bool input_left = false;
bool input_right = false;

/** Create the base for the boxes to land */
void createGround(b2World& World, float X, float Y);

/** Create the boxes */
void createBox(b2World& World, int MouseX, int MouseY);

/** Bots */
std::vector<float> landerBot(std::vector<float> obs);

void createBox(b2World& World, int MouseX, int MouseY)
{
    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(MouseX/SCALE, MouseY/SCALE);
    BodyDef.type = b2_dynamicBody;
    b2Body* Body = World.CreateBody(&BodyDef);

    b2PolygonShape Shape;
    Shape.SetAsBox((32.f/2)/SCALE, (32.f/2)/SCALE);
    b2FixtureDef FixtureDef;
    FixtureDef.density = 1.f;
    FixtureDef.friction = 0.7f;
    FixtureDef.shape = &Shape;
    Body->CreateFixture(&FixtureDef);
}

void createGround(b2World& World, float X, float Y)
{
    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(X/SCALE, Y/SCALE);
    BodyDef.type = b2_staticBody;
    b2Body* Body = World.CreateBody(&BodyDef);

    b2PolygonShape Shape;
    Shape.SetAsBox((800.f/2)/SCALE, (16.f/2)/SCALE);
    b2FixtureDef FixtureDef;
    FixtureDef.density = 0.f;
    FixtureDef.shape = &Shape;
    Body->CreateFixture(&FixtureDef);
}


int game(bool bot)
{
    LunarLander env(false);

    // discrete
    int actionSpace = (int)(env.actionSpace.at(0)); // = 4 for lunar lander (noop, left engine, main engine, right engine)

    EnvData envData = env.reset();

    // random sampler
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(0.0, 4.0);


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
        // action = choose action based on envData.observation

        //std::vector<float> action;
        //action.push_back(std::floor(dis(gen)));
        //std::cout << action.front() << std::endl;

        env.render();
        if (!bot){
        // update the sf::RenderWindow with the new location of stuff (redraw basically)
            float arr[] = {0.0f};
            if (input_up == true){
                qDebug("up");
                arr[0] = 2.0f;
                std::vector<float> rtn (arr, arr + sizeof(arr) / sizeof(arr[0]) );
                envData = env.step(rtn);
            }
            else if (input_left == true){
                qDebug("left");
                arr[0] = 1.0f;
                std::vector<float> rtn (arr, arr + sizeof(arr) / sizeof(arr[0]) );
                envData = env.step(rtn);
            }
            else if (input_right == true){
                qDebug("right");
                arr[0] = 3.0f;
                std::vector<float> rtn (arr, arr + sizeof(arr) / sizeof(arr[0]) );
                envData = env.step(rtn);
            }
            else{
                arr[0] = 0.0f;
                std::vector<float> rtn (arr, arr + sizeof(arr) / sizeof(arr[0]) );
                envData = env.step(rtn);
            }

        }
        else
            envData = env.step(landerBot(observation));
        observation = envData.observation;
        episodeReward += envData.reward;

//        std::cout << "obs: ";
//        for (std::vector<float>::const_iterator i = envData.observation.begin(); i != envData.observation.end(); ++i)
//            std::cout << *i << ' ';
//        std::cout << std::endl;

        // lander has crashed or landed successfully or timed out
        if (envData.done) {
            envData = env.reset();
            std::cout << "done, ep reward = " << episodeReward << std::endl;
            episodeReward = 0;
            //std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }

    return 0;
}

std::vector<float> landerBot(std::vector<float> obs){
    //observation values: x, y, xvel, yvel, angle, anglevel, leftleg, rightleg
    //(noop, left engine, main engine, right engine)
    // -anglevel : right , +anglevel : left
    float arr[] = {1.0f};
    if (obs[3] < -.4){
        if(obs[2] < -.85){
            arr[0] = 3.0f;
        }
        else if(obs[2] > .85){
            arr[0] = 1.0f;
        }
        else
            arr[0] = 2.0f;
    }
    else if (obs[4] < -.2){
        arr[0] = 1.0f;
    }
    else if (obs[4] > .2){
        arr[0] = 3.0f;
    }
    else if (obs[0] < -0.1){
        arr[0] = 3.0f;
    }
    else if (obs[0] > 0.1){
        arr[0] = 1.0f;
    }
    else{
        arr[0] = 0.0f;
    }
    std::vector<float> rtn (arr, arr + sizeof(arr) / sizeof(arr[0]) );
    return rtn;

}
