#include "lunarlander.h"

LunarLander::LunarLander() {
    // create lander shape
    b2Vec2 landerXY[6];
    landerXY[0] = b2Vec2(-14, +17);
    landerXY[1] = b2Vec2(-17, 0);
    landerXY[2] = b2Vec2(-17, -10);
    landerXY[3] = b2Vec2(+17, -10);
    landerXY[4] = b2Vec2(+17, 0);
    landerXY[5] = b2Vec2(+14, +17);
    for (int i = 0; i < 6; i++) {
        landerXY[i] *= 1.0/SCALE;
    }
    landerShape_.Set(landerXY, 6);

    b2Vec2 gravity(0.f, 9.8f);
    world_ = b2World(gravity);
    moon_ = nullptr;
    lander_ = nullptr;
    leftLeg_ = nullptr;
    rightLeg_ = nullptr;
    contactDetector_ = nullptr;
}

LunarLander::~LanderLander() {
    destroy();
}

LunarLander::destroy() {
    // if things haven't been created, we don't need to destroy them.
    if (contactDetector_ == nullptr) {
        return;
    }

    free(contactDetector_)
    contactDetector_ = nullptr;

    world_.DestroyBody(moon_);
    world_.DestroyBody(lander_);
    world_.DestroyBody(leftLeg_);
    world_.DestroyBody(rightLeg_);
    // not sure if necessary
    moon_ = nullptr;
    lander_ = nullptr;
    leftLeg_ = nullptr;
    rightLeg_ = nullptr;

    destroyParticles(all=true);
}


EnvData LunarLander::reset() {
    destroy();
    contactDetector_ = new ContactDetector(this);
    world_.SetContactListener(contactDetector_);
    gameOver_ = false;
    prevShaping_ = 0;

    int H = VIEWPORT_H/SCALE;
    int W = VIEWPORT_W/SCALE;

    // generate terrain
    int CHUNKS = 11;
    float height[CHUNK+1];
    float chunkX[CHUNK];

    // random sample a bunch of heights 0 to H/2
    std::uniform_real_distribution<> heightSampler(0, H/2);
    for (int i = 0; i < 12; i++) {
        height[i] = heightSampler(gen);
    }
    for (i = 0; i < 11; i++) {
        chunkX[i] = i * (W/(CHUNKS-1));
    }

    // get x location and y height for where we are aiming to land
    int helipadX1 = chunkX[CHUNKS/2 - 1];
    int helipadX2 = chunkX[CHUNKS/2 + 1];
    int helipadY = H/4;

    // set the region near the goal to be flat
    height[CHUNKS/2 - 2] = helipadY;
    height[CHUNKS/2 - 1] = helipadY;
    height[CHUNKS/2 + 0] = helipadY;
    height[CHUNKS/2 + 1] = helipadY;
    height[CHUNKS/2 + 2] = helipadY;

    float smoothY[CHUNKS];
    for (i = 1; i < CHUNKS-1; i++) {
        smoothY = 0.33*(height[i-1] + height[i+0] + height[i+1]);
    }

    // create moon
    b2BodyDef moonBodyDef;
    moonBodyDef.type = b2_staticBody;
    moon_ = world.CreateBody(&moonBodyDef);
    b2EdgeShape moonEdgeShape;
    b2Vec2 v1(0.0f, 0.0f);
    b2Vec2 v2((float) W, 0.0f);
    moonEdgeShape.Set(v1, v2);
    moon_->CreateFixture(moonEdgeShape, 1.0);

    skyPolys_.clear();
    for (i = 0; i < CHUNKS-1; i++) {
        b2Vec2 p1(chunkX[i], smoothY[i]);
        b2Vec2 p2(chunkX[i+1], smoothY[i+1]);
        b2EdgeShape groundEdge;
        groundEdge.Set(p1, p2);
        b2FixtureDef groundDef;
        groundDef.shape = &groundEdge;
        groundDef.density = 0;
        groundDef.friction = 0.1;
        moon_->CreateFixture(groundDef);

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
    landerFixtureDef.shape = landerShape_;
    landerFixtureDef.density = 5.0;
    landerFixtureDef.friction = 0.1;
    landerFixtureDef.filter.categoryBits = 0x0010;
    landerFixtureDef.filter.maskBits = 0x001; // collide only with the ground
    landerFixtureDef.restitution = 0.0; // 0.99 bouncy

    lander_ = world_.CreateBody(&landerBodyDef);
    lander_->CreateFixture(landerFixtureDef);

    std::uniform_real_distribution<> forceSampler(-INITIAL_RANDOM, INITIAL_RANDOM);
    lander_->ApplyForceToCenter(forceSampler(gen), forceSampler(gen), true);


    // create legs
    b2Body legs[2];
    legs[0] = leftLeg_;
    legs[1] = rightLeg_;

    for (i = 0; i < 2; i++) {
      b2BodyDef legBodyDef;
      legBodyDef.type = b2_dynamicBody;
      legBodyDef.position = b2Vec2(VIEWPORT_W / SCALE / 2.0 - ((i-1)*LEG_AWAY)/SCALE, initLanderY);
      legBodyDef.angle = (i-1)*0.05;
      b2FixtureDef legFixtureDef;
      b2PolygonShape legShape;
      legShape.SetAsBox(LEG_W/SCALE, LEG_H/SCALE);
      legFixtureDef.shape = legShape;
      legFixtureDef.density = 1.0;
      legFixtureDef.restitution = 0.0;
      legFixtureDef.filter.categoryBits = 0x0020;
      legFixtureDef.filter.maskBits = 0x001;

      legs[i] = world_.CreateBody(&landerBodyDef);
      legs[i]->CreateFixture(landerFixtureDef);


      b2RevoluteJointDef rjd;
      rjd.bodyA = lander_;
      rjd.bodyB = leg;
      rjd.localAnchorA = b2Vec2(0, 0);
      rjd.localAnchorB = b2Vec2((i-1)*LEG_AWAY/SCALE, LEG_DOWN/SCALE);
      rjd.enableMotor = True;
      rjd.enableLimit = True;
      rjd.maxMotorTorque = LEG_SPRING_TORQUE;
      rjd.motorSpeed += 0.3*(i-1);

      if (i == 0) {
          rjd.lowerAngle = 0.9 - 0.5;
          rjd.upperAngle = 0.9;
      }
      else {
          rjd.lowerAngle = -0.9;
          rjd.upperAngle = -0.9 + 0.5;
      }

      world_.CreateJoint(&rjd);
    }

    drawList_.clear();
    drawList_.push_back(lander_);
    drawList_.push_back(rightLeg_);
    drawList_.push_back(leftLeg_);

    if (continuous_) {
        return step(0);
    }
    else {
        return step(0);
    }
}

EnvData LunarLander::step(std::vector<float>) {



}

void LunarLander::render() {

}
