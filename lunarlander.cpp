#include "lunarlander.h"
#include <iostream>

float clip(float n, float lower, float upper) {
  return std::max(lower, std::min(n, upper));
}

/*
 * Custom handler for collisions so we can handle them in our game
 */
class ContactDetector : public b2ContactListener
{
public:
    LunarLander* env_;
    ContactDetector() {
        // keep env_ member variable so we can access data
        //env_ = env;
    }

    void SetEnv(LunarLander* env) {
        env_ = env;
    }

    /*
     * Event handler called when two Box2D bodies collide (contact)
     */
    void BeginContact(b2Contact* contact) {
        b2Body* bodyA = contact->GetFixtureA()->GetBody();
        b2Body* bodyB = contact->GetFixtureB()->GetBody();
        // Lander crashed into the ground
        if (env_->lander_ == bodyA || env_->lander_ == bodyB) {
            env_->gameOver_ = true;
        }
        // legs are touching the ground (so the bot knows it has reached the surface)
        if (env_->leftLeg_ == bodyA || env_->leftLeg_ == bodyB) {
            env_->leftLegGroundContact_ = true;
        }
        if (env_->rightLeg_ == bodyA || env_->rightLeg_ == bodyB) {
            env_->rightLegGroundContact_ = true;
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
        if (env_->leftLeg_ == bodyA || env_->leftLeg_ == bodyB) {
            env_->leftLegGroundContact_ = false;
        }
        if (env_->rightLeg_ == bodyA || env_->rightLeg_ == bodyB) {
            env_->rightLegGroundContact_ = false;
        }
    }
};


LunarLander::LunarLander(bool continuous): gen_(std::random_device()()) {
    // create lander shape
    LANDERPOLY[0] = b2Vec2(-14, +17);
    LANDERPOLY[1] = b2Vec2(-17, 0);
    LANDERPOLY[2] = b2Vec2(-17, -10);
    LANDERPOLY[3] = b2Vec2(+17, -10);
    LANDERPOLY[4] = b2Vec2(+17, 0);
    LANDERPOLY[5] = b2Vec2(+14, +17);
    for (int i = 0; i < 6; i++) {
        LANDERPOLY[i] *= 1.0/SCALE;
    }


    // set up action space
    actionSpace.clear();
    if (continuous) {
        // Action is two floats [main engine, left-right engines].
        // Main engine: -1..0 off, 0..+1 throttle from 50% to 100% power. Engine can't work with less than 50% power.
        // Left-right:  -1.0..-0.5 fire left engine, +0.5..+1.0 fire right engine, -0.5..0.5 off
        actionSpace.push_back(1.0);
        actionSpace.push_back(1.0);
    }
    else {
        // discrete takes a single action of 4 possible values (0,1,2,3 = noop, left engine, main engine, right engine)
        actionSpace.push_back(4);
    }

    world_ = new b2World(b2Vec2(0.f, 9.8f));
    moon_ = nullptr;
    lander_ = nullptr;
    leftLeg_ = nullptr;
    rightLeg_ = nullptr;
    contactDetector_ = nullptr;
    viewer_ = nullptr;
}

LunarLander::~LunarLander() {
    destroy();
    free(world_);
}

b2Body* LunarLander::createParticle(float mass, float x, float y, float* power) {
    b2BodyDef particleBodyDef;
    particleBodyDef.type = b2_dynamicBody;
    particleBodyDef.position = b2Vec2(x, y);
    particleBodyDef.angle = 0.0;
    b2FixtureDef particleFixtureDef;
    b2CircleShape circle;
    circle.m_p = b2Vec2(0.0, 0.0);
    circle.m_radius = 2.0/SCALE;
    particleFixtureDef.shape = &circle;
    particleFixtureDef.density = mass;
    particleFixtureDef.friction = 0.1;
    particleFixtureDef.filter.categoryBits = 0x0100;
    particleFixtureDef.filter.maskBits = 0x001;
    particleFixtureDef.restitution = 0.3;

    b2Body* particle = world_->CreateBody(&particleBodyDef);
    particle->SetUserData(power);
    particle->CreateFixture(&particleFixtureDef);

    particles_.push_back(particle);

    return particle;
}

void LunarLander::destroyParticles(bool all) {
    float* power = (float*) (particles_.front()->GetUserData());

    while (!particles_.empty() && (all || *power < 0.0)) {
        // clean user data
        free(power);
        // destroy the particle body
        b2Body* particle = particles_.front();
        world_->DestroyBody(particle);
        particles_.pop_front();
        power = (float*) (particles_.front()->GetUserData());
    }
}

void LunarLander::destroy() {
    // if things haven't been created, we don't need to destroy them.
    if (contactDetector_ == nullptr) {
        return;
    }

    free(contactDetector_);
    contactDetector_ = nullptr;

    world_->DestroyBody(moon_);
    world_->DestroyBody(lander_);
    world_->DestroyBody(leftLeg_);
    world_->DestroyBody(rightLeg_);
    // not sure if necessary
    moon_ = nullptr;
    lander_ = nullptr;
    leftLeg_ = nullptr;
    rightLeg_ = nullptr;

    destroyParticles(true);
}


EnvData LunarLander::reset() {
    destroy();
    contactDetector_ = new ContactDetector;
    contactDetector_->SetEnv(this);
    world_->SetContactListener(contactDetector_);
    gameOver_ = false;
    prevShaping_ = 0;

    int H = VIEWPORT_H/SCALE;
    int W = VIEWPORT_W/SCALE;

    // generate terrain
    const int CHUNKS = 11;
    float height[CHUNKS+1];
    float chunkX[CHUNKS];

    // random sample a bunch of heights 0 to H/2
    std::uniform_real_distribution<> heightSampler(0, H/2);
    int i;
    for (i = 0; i < 12; i++) {
        height[i] = heightSampler(gen_);
    }
    for (i = 0; i < 11; i++) {
        chunkX[i] = i * (W/(CHUNKS-1));
    }

    // get x location and y height for where we are aiming to land
    helipadX1_ = chunkX[CHUNKS/2 - 1];
    helipadX2_ = chunkX[CHUNKS/2 + 1];
    helipadY_ = H/4;

    // set the region near the goal to be flat
    height[CHUNKS/2 - 2] = helipadY_;
    height[CHUNKS/2 - 1] = helipadY_;
    height[CHUNKS/2 + 0] = helipadY_;
    height[CHUNKS/2 + 1] = helipadY_;
    height[CHUNKS/2 + 2] = helipadY_;

    float smoothY[CHUNKS];
    for (i = 1; i < CHUNKS-1; i++) {
        smoothY[i] = 0.33*(height[i-1] + height[i+0] + height[i+1]);
    }

    // create moon
    b2BodyDef moonBodyDef;
    moonBodyDef.type = b2_staticBody;
    moon_ = world_->CreateBody(&moonBodyDef);
    b2EdgeShape* moonEdgeShape = new b2EdgeShape;
    b2Vec2 v1(0.0f, 0.0f);
    b2Vec2 v2((float) W, 0.0f);
    moonEdgeShape->Set(v1, v2);
    moon_->CreateFixture(moonEdgeShape, 1.0);

    skyPolys_.clear();
    for (i = 0; i < CHUNKS-1; i++) {
        b2Vec2 p1(chunkX[i], smoothY[i]);
        b2Vec2 p2(chunkX[i+1], smoothY[i+1]);
        b2EdgeShape* groundEdge = new b2EdgeShape;
        groundEdge->Set(p1, p2);
        b2FixtureDef groundDef;
        groundDef.shape = groundEdge;
        groundDef.density = 0;
        groundDef.friction = 0.1;
        moon_->CreateFixture(&groundDef);

        // stuff for drawing sky
        b2Vec2 skyTop1(p1.x, (float) H);
        b2Vec2 skyTop2(p2.x, (float) H);

        std::tuple<b2Vec2, b2Vec2, b2Vec2, b2Vec2> polyTupe = std::make_tuple(p1, p2, skyTop1, skyTop2);
        skyPolys_.push_back(polyTupe);
    }


    // create lander
    float initLanderY = VIEWPORT_H / SCALE;
    b2BodyDef landerBodyDef;
    landerBodyDef.type = b2_dynamicBody;
    landerBodyDef.position = b2Vec2((VIEWPORT_W / SCALE)/2.0, initLanderY);
    landerBodyDef.angle = 0.0;
    b2FixtureDef landerFixtureDef;

    b2PolygonShape* landerShape_ = new b2PolygonShape;
    landerShape_->Set(LANDERPOLY, 6);
    landerFixtureDef.shape = landerShape_;
    landerFixtureDef.density = 5.0;
    landerFixtureDef.friction = 0.1;
    landerFixtureDef.filter.categoryBits = 0x0010;
    landerFixtureDef.filter.maskBits = 0x001; // collide only with the ground
    landerFixtureDef.restitution = 0.0; // 0.99 bouncy

    lander_ = world_->CreateBody(&landerBodyDef);
    lander_->CreateFixture(&landerFixtureDef);

    std::uniform_real_distribution<> forceSampler(-INITIAL_RANDOM, INITIAL_RANDOM);
    float f1 = forceSampler(gen_);
    float f2 = forceSampler(gen_);
    lander_->ApplyForceToCenter(b2Vec2(forceSampler(gen_), forceSampler(gen_)), true);

    b2BodyDef leftLegBodyDef;
    leftLegBodyDef.type = b2_dynamicBody;
    leftLegBodyDef.position = b2Vec2(VIEWPORT_W / SCALE / 2.0 - ((i-1)*LEG_AWAY)/SCALE, initLanderY);
    leftLegBodyDef.angle = (i-1)*0.05;
    b2FixtureDef leftLegFixtureDef;
    b2PolygonShape* leftLegShape = new b2PolygonShape;
    leftLegShape->SetAsBox(LEG_W/SCALE, LEG_H/SCALE);
    leftLegFixtureDef.shape = leftLegShape;
    leftLegFixtureDef.density = 1.0;
    leftLegFixtureDef.restitution = 0.0;
    leftLegFixtureDef.filter.categoryBits = 0x0020;
    leftLegFixtureDef.filter.maskBits = 0x001;

	leftLeg_ = world_->CreateBody(&landerBodyDef);
    leftLeg_->CreateFixture(&landerFixtureDef);


    b2RevoluteJointDef leftLegRJD;
    leftLegRJD.bodyA = lander_;
    leftLegRJD.bodyB = leftLeg_;
    leftLegRJD.localAnchorA = b2Vec2(0, 0);
    leftLegRJD.localAnchorB = b2Vec2((i-1)*LEG_AWAY/SCALE, LEG_DOWN/SCALE);
    leftLegRJD.enableMotor = true;
    leftLegRJD.enableLimit = true;
    leftLegRJD.maxMotorTorque = LEG_SPRING_TORQUE;
    leftLegRJD.motorSpeed += 0.3*(i-1);

    if (i == 0) {
        leftLegRJD.lowerAngle = 0.9 - 0.5;
        leftLegRJD.upperAngle = 0.9;
    }
    else {
        leftLegRJD.lowerAngle = -0.9;
        leftLegRJD.upperAngle = -0.9 + 0.5;
    }

    world_->CreateJoint(&leftLegRJD);

    b2BodyDef rightLegBodyDef;
    rightLegBodyDef.type = b2_dynamicBody;
    rightLegBodyDef.position = b2Vec2(VIEWPORT_W / SCALE / 2.0 - ((i-1)*LEG_AWAY)/SCALE, initLanderY);
    rightLegBodyDef.angle = (i-1)*0.05;
    b2FixtureDef rightLegFixtureDef;
    b2PolygonShape* rightLegShape = new b2PolygonShape;
    rightLegShape->SetAsBox(LEG_W/SCALE, LEG_H/SCALE);
    rightLegFixtureDef.shape = rightLegShape;
    rightLegFixtureDef.density = 1.0;
    rightLegFixtureDef.restitution = 0.0;
    rightLegFixtureDef.filter.categoryBits = 0x0020;
    rightLegFixtureDef.filter.maskBits = 0x001;

    rightLeg_ = world_->CreateBody(&landerBodyDef);
    rightLeg_->CreateFixture(&landerFixtureDef);


    b2RevoluteJointDef rightLegRJD;
    rightLegRJD.bodyA = lander_;
    rightLegRJD.bodyB = rightLeg_;
    rightLegRJD.localAnchorA = b2Vec2(0, 0);
    rightLegRJD.localAnchorB = b2Vec2((i-1)*LEG_AWAY/SCALE, LEG_DOWN/SCALE);
    rightLegRJD.enableMotor = true;
    rightLegRJD.enableLimit = true;
    rightLegRJD.maxMotorTorque = LEG_SPRING_TORQUE;
    rightLegRJD.motorSpeed += 0.3*(i-1);

    if (i == 0) {
        rightLegRJD.lowerAngle = 0.9 - 0.5;
        rightLegRJD.upperAngle = 0.9;
    }
    else {
        rightLegRJD.lowerAngle = -0.9;
        rightLegRJD.upperAngle = -0.9 + 0.5;
    }

    world_->CreateJoint(&rightLegRJD);





    drawList_.clear();
    drawList_.push_back(lander_);
    drawList_.push_back(rightLeg_);
    drawList_.push_back(leftLeg_);

    std::vector<float> action; // do nothing
    if (continuous_) {
        action.push_back(0.0);
        action.push_back(0.0);
    }
    else {
        action.push_back(0);
    }
    return step(action);
}

EnvData LunarLander::step(std::vector<float> action) {
    EnvData envData;

    if (continuous_) {
        //assert(action.size() == 2);
    }
    else {
        assert(action.size() == 1);
        assert(action.front() < actionSpace.front());
    }

    // Engines
    b2Vec2 tip(sin(lander_->GetAngle()), cos(lander_->GetAngle()));
    b2Vec2 side(-tip.y, tip.x);

    std::uniform_real_distribution<> dispersionSampler(-1.0, 1.0);
    b2Vec2 dispersion(dispersionSampler(gen_) / SCALE, dispersionSampler(gen_) / SCALE);

    // allocate a float like this so it doesn't get destroyed when we leave scope
    // (we need it later in the particle body)
    float* mPower = new float[1]();
    mPower[0] = 0.0;


    // Main engine
    if ((continuous_ && action.front() > 0.0) || (!continuous_ && ((int)action.front()) == 2)) {
        if (continuous_) {
            mPower[0] = 0.5 * clip(action.front(), 0.0, 1.0);
            assert (mPower[0] >= 0.5 && mPower[0] <= 1.0);
        }
        else {
            mPower[0] = 1.0;
        }

        // 4 is move a bit downwards, +-2 for randomness
        float ox = tip.x * (4/SCALE + 2*dispersion.x + side.x*dispersion.y);
        float oy = tip.y * (4/SCALE + 2*dispersion.x - side.y*dispersion.y);

        b2Vec2 impulsePos(lander_->GetPosition().x + ox, lander_->GetPosition().y + oy);
        b2Body* particle = createParticle(3.5, impulsePos.x, impulsePos.y, mPower);

        // newton's 3rd law. cool.
        particle->ApplyLinearImpulse(b2Vec2(ox*MAIN_ENGINE_POWER*mPower[0], oy*MAIN_ENGINE_POWER*mPower[0]), impulsePos, true);
        lander_->ApplyLinearImpulse(b2Vec2(-ox*MAIN_ENGINE_POWER*mPower[0], -oy*MAIN_ENGINE_POWER*mPower[0]), impulsePos, true);
    }

    // Side engines
    // allocate a float like this so it doesn't get destroyed when we leave scope
    // (we need it later in the particle body)
    float* sPower = new float[1]();
    sPower[0] = 0.0;

    if ( (continuous_ && std::abs(action.back() > 0.5)) || ((!continuous_ && ((int)action.front() == 1 || (int)action.front() == 3) )) ) {
        int direction;
        if (continuous_) {
            direction = 1 + -2*std::signbit(action.back());
            sPower[0] = clip(std::abs(action.back()), 0.5, 1.0);
            assert(sPower[0] >= 0.5 && sPower[0] <= 1.0);
        }
        else {
            direction = ((int)action.front())-2;
            sPower[0] = 1.0;
        }


        float ox = tip.x * dispersion.x + side.x * (3*dispersion.y + direction * SIDE_ENGINE_AWAY/SCALE);
        float oy = -tip.y * dispersion.x + side.y * (3*dispersion.y + direction * SIDE_ENGINE_AWAY/SCALE);

        b2Vec2 landerXY = lander_->GetPosition();
        b2Vec2 impulsePos(landerXY.x + ox - tip.x*17.0/SCALE, landerXY.y + oy + tip.y*SIDE_ENGINE_HEIGHT/SCALE);

        b2Body* particle = createParticle(0.7, impulsePos.x, impulsePos.y, sPower);

        // newton's 3rd law. cool.
        particle->ApplyLinearImpulse(b2Vec2(ox*SIDE_ENGINE_POWER*sPower[0],  oy*SIDE_ENGINE_POWER*sPower[0]), impulsePos, true);
        lander_->ApplyLinearImpulse(b2Vec2(-ox*SIDE_ENGINE_POWER*sPower[0], -oy*SIDE_ENGINE_POWER*sPower[0]), impulsePos, true);
    }

    world_->Step(1.0/FPS, 6*30, 2*30);

    b2Vec2 pos = lander_->GetPosition();
    b2Vec2 vel = lander_->GetLinearVelocity();

    std::vector<float> state;
    state.push_back((pos.x - VIEWPORT_W/SCALE/2.0) / (VIEWPORT_W/SCALE/2.0)); // x position
    state.push_back((pos.y - (helipadY_+LEG_DOWN/SCALE)) / (VIEWPORT_H/SCALE/2.0)); // y position (above landing)
    state.push_back(vel.x*(VIEWPORT_W/SCALE/2)/FPS);
    state.push_back(vel.y*(VIEWPORT_H/SCALE/2)/FPS);
    state.push_back(lander_->GetAngle());
    state.push_back(20.0*lander_->GetAngularVelocity()/FPS);
    state.push_back((float) leftLegGroundContact_);
    state.push_back((float) rightLegGroundContact_);
    assert(state.size() == 8);

    float reward = 0.0;

    // Punish (give negative reward) for large position xy and velocity xy values
    // Add ten points for leg contact
    float shaping = - 100*std::sqrt(state.at(0)*state.at(0) + state.at(1)*state.at(1)) \
                    - 100*std::sqrt(state.at(2)*state.at(2) + state.at(3)*state.at(3)) \
                    - 100*std::abs(state.at(4)) + 10*state[6] + 10*state[7];

    // don't keep giving the same large reward for this. give it like once
    if (prevShaping_ != 0.0) {
        reward = shaping - prevShaping_;
    }
    prevShaping_ = shaping;

    reward -= mPower[0]*0.30; // less fuel spent is better, about -30 for heuristic landing
    reward -= sPower[0]*0.03;

    bool done = false;
    if (gameOver_ || std::abs(state.at(0) >= 1.0)) {
        done = true;
        reward = -100; // we crashed :(
    }
    if (!lander_->IsAwake()) {
        done = true;
        reward = 100; // we landed safely :)
    }

    envData.observation = state;
    envData.reward = reward;
    envData.done = done;
    return envData;
}

void LunarLander::render() {

    // TODO:

    // set viewer if null
    if (viewer_ == nullptr){
        std::cout << "hi" << std::endl;
        viewer_ = new sf::RenderWindow(sf::VideoMode(600, 400), "test");
    }

    //std::cout << skyPolys_.size();
    sf::Texture BoxTexture;
    BoxTexture.loadFromFile("../cs3505-f17-a8-edu-app-matwilso/box.png");

    // create an empty shape
    sf::ConvexShape convex;

    // resize it to 10 points
    convex.setPointCount(11);
    convex.setFillColor(sf::Color().Black);

    //draw moon
    for (int i = 0; i < skyPolys_.size(); i++){
        sf::Sprite Sprite;
        convex.setPoint(i, sf::Vector2f(std::get<0>(skyPolys_[i]).x*SCALE, 400 - std::get<0>(skyPolys_[i]).y*SCALE));
    }
    convex.setPoint(10, sf::Vector2f(600, 400));
    viewer_->draw(convex);

    //draw flags
    sf::RectangleShape flag, flag2;
    flag.setSize(sf::Vector2f(3, -(helipadY_ + 1000/SCALE)));
    flag2.setSize(sf::Vector2f(3, -(helipadY_ + 1000/SCALE)));
    flag.setPosition(sf::Vector2f(SCALE*helipadX1_, 400 -helipadY_*SCALE));
    flag2.setPosition(sf::Vector2f(SCALE*helipadX2_, 400 -helipadY_*SCALE));
    flag.setFillColor(sf::Color().Red);
    flag2.setFillColor(sf::Color().Red);
    viewer_->draw(flag);
    viewer_->draw(flag2);

    //draw lander
    sf::RectangleShape test;
    test.setSize(sf::Vector2f(16, 16));
    test.setPosition(drawList_[0]->GetPosition().x*SCALE, drawList_[0]->GetPosition().y);
    test.setFillColor(sf::Color().Blue);
    viewer_->draw(test);

    //std::cout << drawList_[1]->GetPosition().x << std::endl;


    //TODO: draw legs

    viewer_->display();
    viewer_->clear(sf::Color(255, 255, 255));

    //sf::RenderWindow Window(sf::VideoMode(800, 600, 32), "Test");

}
