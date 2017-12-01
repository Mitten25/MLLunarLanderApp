#ifndef _LUNARLANDER_H_
#include "env.h"
#include <random>

/*
 * LunarLander Game Implementation
 *
 * Ported from: https://github.com/openai/gym/blob/master/gym/env_s/box2d/lunar_lander.py
 *
 * STYLE:
 * trailing underscores = class member variable
 * (nice so you know what belongs to the class, and what is just a param or temp)
 *
 */


const int FPS    = 50;
const int SCALE  = 30.0;   // affects how fast-paced the game is, forces should be adjusted as well

const float MAIN_ENGINE_POWER  = 13.0;
const float SIDE_ENGINE_POWER  =  0.6;

const float INITIAL_RANDOM = 1000.0;   // Set 1500 to make game harder

b2Vec2 LANDER_POLY[6];

const int LEG_AWAY = 20;
const int LEG_DOWN = 18;
const int LEG_W = 2;
const int LEG_H = 8;
const int LEG_SPRING_TORQUE = 40;

const float SIDE_ENGINE_HEIGHT = 14.0;
const float SIDE_ENGINE_AWAY   = 12.0;

const int VIEWPORT_W = 600;
const int VIEWPORT_H = 400;

/*
 * Custom handler for collisions so we can handle them in our game
 */
class ContactDetector : public b2ContactListener
{
public:
    LunarLander env_;
    void ContactDetector::ContactDetector(LunarLander env) {
        // keep env_ member variable so we can access data
        env_ = env;
    }
    /*
     * Event handler called when two Box2D bodies collide (contact)
     */
    void BeginContact(b2Contact* contact) {
        b2Body* bodyA = contact->GetFixtureA()->GetBody();
        b2Body* bodyB = contact->GetFixtureB()->GetBody();
        // Lander crashed into the ground
        if (env_.lander == bodyA || env_.lander == bodyB) {
            env_.gameOver = true;
        }
        // legs are touching the ground (so the bot knows it has reached the surface)
        if (env_.leftLeg == bodyA || env_.leftLeg == bodyB) {
            env_.leftLegGroundContact = true;
        }
        if (env_.rightLeg == bodyA || env_.rightLeg == bodyB) {
            env_.rightLegGroundContact = true;
        }
    }
    /*
     * Event handler called when two Box2D bodies stop contacting
     */
    void EndContact(b2Contact* contact) {
        // these
        b2Body* bodyA = contact->GetFixtureA()->GetBody();
        b2Body* bodyB = contact->GetFixtureB()->GetBody();

        // check if either leg ended contact with the ground
        if (env_.leftLeg == bodyA || env_.leftLeg == bodyB) {
            env_.leftLegGroundContact = false;
        }
        if (env_.rightLeg == bodyA || env_.rightLeg == bodyB) {
            env_.rightLegGroundContact = false;
        }
    }
};


/*
 * LunarLander to match OpenAI Gym-like API
 */

class LunarLander : Env
{
    friend class ContactDetector; // make it so ContactDectector can access private member variables
private:
    /*
     * Clean up all resources for next episode
     */
    void destroy(void);

    /*
     *
     */
    void destroyParticles(bool all);

    /*
     *
     */
    void createParticle(float mass, float x, float y, float throttle);

    b2World world_; // main Box2D world object

    b2Body* moon_; // body of lunar surface
    std::vector<b2Body*> particles_; // particles that shoot out of the rocket (just for looks)
    b2Body* lander_; // body of lunar lander
    b2PolygonShape landerShape_; // shape of lander for physics and drawing
    b2Body* leftLeg_; // left leg of lunar lander
    b2Body* rightLeg_; // right leg of lunar lander
    bool leftLegGroundContact_; // if the left leg is touching the ground
    bool rightLegGroundContact_; // if the right leg is touching the ground
    ContactDetector* contactDetector_; // Box2D object for handling collisions
    bool gameOver_; // this will get set when the thing crashes or something, so we know when to end things
    float prevShaping_; // used for setting reward

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()



public:
    LunarLander::LunarLander(bool continuous);
    std::pair<float, float> observationRange;
    //std::tuple<std::vector<float>, float, bool> reset(float);
    //std::tuple<std::vector<float>, float, bool> step();
    //void render();
    //TODO: close() maybe
};


#endif
#define _LUNARLANDER_H_
