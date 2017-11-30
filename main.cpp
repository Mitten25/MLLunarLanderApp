#include "mainwindow.h"
#include <QApplication>
#include "Box2D/Box2D.h"
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "QtMath"
#include "tuple"
#undef main
static const float SCALE = 30.f;

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
/*
class Env{
public:
    std::vector<int> action_space;
    std::vector<int> observation_space;
    std::vector<int> reward_range;

    //virtual std::tuple<std::vector<int>, float, bool> step();
    //virtual std::vector<int> reset();
    //virtual void render();
    //virtual void close();
    //virtual void seed();
};
*/
class LunarLander{
private:
    b2World world;
    b2Body* lander;

public:
    LunarLander():world(b2Vec2()){}

    LunarLander(b2World &World):world(World){
        //observation_space = new std::vector<int>;
        //action_space =
        lander = NULL;

        reset();
    }

    void destroy(){
        world.DestroyBody(lander);
        world.SetContactListener(NULL);
    }

    std::vector<int> reset(){
        destroy();
        int W = 800;
        int H = 600;
        int initial_y = H/SCALE;

        b2BodyDef BodyDef;
        BodyDef.position = b2Vec2((W/SCALE)/2, initial_y);
        BodyDef.angle = 0.0f;
        BodyDef.type = b2_dynamicBody;
        //TODO: add fixtures
        lander = world.CreateBody(&BodyDef);
        //TODO: add initial force lander.ApplyForceToCenter();
        std::vector<int> ret;
        return ret;
    }
/*
    std::tuple<std::vector<int>, float, bool> step(){
        std::tuple<double, double> tip (std::make_tuple(qSin(lander->GetAngle()), qCos(lander->GetAngle())));
        std::tuple<double, double> side (std::make_tuple(-std::get<0>(tip), std::get<0>(tip)));
        std::tuple<std::vector<int>, float, bool> ret;
        return ret;
    }
*/
};

int main()
{
    //QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    /** Prepare the window */
    sf::RenderWindow Window(sf::VideoMode(800, 600, 32), "Test");
    Window.setFramerateLimit(60);

    /** Prepare the world */
    b2Vec2 Gravity(0.f, 9.8f);
    b2World World(Gravity);
    createGround(World, 400.f, 500.f);

    /** Prepare textures */
    sf::Texture GroundTexture;
    sf::Texture BoxTexture;
    GroundTexture.loadFromFile("../cs3505-f17-a8-edu-app-matwilso/ground.png");
    BoxTexture.loadFromFile("../cs3505-f17-a8-edu-app-matwilso/box.png");

    LunarLander* LL = new LunarLander(World);
    LL->reset();
    while (Window.isOpen())
    {/*
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            int MouseX = sf::Mouse::getPosition(Window).x;
            int MouseY = sf::Mouse::getPosition(Window).y;
            createBox(World, MouseX, MouseY);
        }*/
        World.Step(1/60.f, 8, 3);

        Window.clear(sf::Color::White);
        int BodyCount = 0;
        for (b2Body* BodyIterator = World.GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
        {
            if (BodyIterator->GetType() == b2_dynamicBody)
            {
                sf::Sprite Sprite;
                Sprite.setTexture(BoxTexture);
                Sprite.setOrigin(16.f, 16.f);
                Sprite.setPosition(SCALE * BodyIterator->GetPosition().x, SCALE * BodyIterator->GetPosition().y);
                Sprite.setRotation(BodyIterator->GetAngle() * 180/b2_pi);
                Window.draw(Sprite);
                ++BodyCount;
            }
            else
            {
                sf::Sprite GroundSprite;
                GroundSprite.setTexture(GroundTexture);
                GroundSprite.setOrigin(400.f, 8.f);
                GroundSprite.setPosition(BodyIterator->GetPosition().x * SCALE, BodyIterator->GetPosition().y * SCALE);
                GroundSprite.setRotation(180/b2_pi * BodyIterator->GetAngle());
                Window.draw(GroundSprite);
            }
        }
        Window.display();
    }

    return 0;
}
