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

/** Create the base for the boxes to land */
void createGround(b2World& World, float X, float Y);

/** Create the boxes */
void createBox(b2World& World, int MouseX, int MouseY);

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


int game()
{
    LunarLander env(false);

    // discrete
    int actionSpace = (int)(env.actionSpace.at(0)); // = 4 for lunar lander (noop, left engine, main engine, right engine)

    EnvData envData = env.reset();

    float episodeReward = 0;
    // loop forever running episodes of lunar lander
    while (1) {
        // action = choose action based on envData.observation
        std::vector<float> action;
        action.push_back(0); // TODO: something better than always 0 (noop)

        // update the sf::RenderWindow with the new location of stuff (redraw basically)

        env.render();
        envData = env.step(action);
        episodeReward += envData.reward;

        std::cout << "obs: ";
        for (std::vector<float>::const_iterator i = envData.observation.begin(); i != envData.observation.end(); ++i)
            std::cout << *i << ' ';
         std::cout << std::endl;



        // lander has crashed or landed successfully or timed out
        if (envData.done) {
            envData = env.reset();
            std::cout << "done, ep reward = " << episodeReward << std::endl;
            episodeReward = 0;
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }



    ///** Prepare the window */
    //sf::RenderWindow Window(sf::VideoMode(800, 600, 32), "Test");
    //Window.setFramerateLimit(60);

    ///** Prepare the world */
    //b2Vec2 Gravity(0.f, 9.8f);
    //b2World World(Gravity);
    //createGround(World, 400.f, 500.f);

    ///** Prepare textures */
    //sf::Texture GroundTexture;
    //sf::Texture BoxTexture;
    //GroundTexture.loadFromFile("../cs3505-f17-a8-edu-app-matwilso/ground.png");
    //BoxTexture.loadFromFile("../cs3505-f17-a8-edu-app-matwilso/box.png");

    //LunarLander* LL = new LunarLander(World);
    //LL->reset();
    //while (Window.isOpen())
    //{/*
    //    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    //    {
    //        int MouseX = sf::Mouse::getPosition(Window).x;
    //        int MouseY = sf::Mouse::getPosition(Window).y;
    //        createBox(World, MouseX, MouseY);
    //    }*/
    //    World.Step(1/60.f, 8, 3);

    //    Window.clear(sf::Color::White);
    //    int BodyCount = 0;
    //    for (b2Body* BodyIterator = World.GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
    //    {
    //        if (BodyIterator->GetType() == b2_dynamicBody)
    //        {
    //            sf::Sprite Sprite;
    //            Sprite.setTexture(BoxTexture);
    //            Sprite.setOrigin(16.f, 16.f);
    //            Sprite.setPosition(SCALE * BodyIterator->GetPosition().x, SCALE * BodyIterator->GetPosition().y);
    //            Sprite.setRotation(BodyIterator->GetAngle() * 180/b2_pi);
    //            Window.draw(Sprite);
    //            ++BodyCount;
    //        }
    //        else
    //        {
    //            sf::Sprite GroundSprite;
    //            GroundSprite.setTexture(GroundTexture);
    //            GroundSprite.setOrigin(400.f, 8.f);
    //            GroundSprite.setPosition(BodyIterator->GetPosition().x * SCALE, BodyIterator->GetPosition().y * SCALE);
    //            GroundSprite.setRotation(180/b2_pi * BodyIterator->GetAngle());
    //            Window.draw(GroundSprite);
    //        }
    //    }
    //    Window.display();
    //}

    return 0;
}
