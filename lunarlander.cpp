#include "lunarlander.h"

sf::RenderWindow Window(sf::VideoMode(800, 600, 32), "Test");
Window.setFramerateLimit(60);

/** Prepare the world */
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


LunarLander::LunarLander() {
    // create lander shape (like a rhombus or something)
    LANDER_POLY[0].Set(-14, +17);
    LANDER_POLY[1].Set(-17, 0);
    LANDER_POLY[2].Set(-17, -10);
    LANDER_POLY[3].Set(+17, -10);
    LANDER_POLY[4].Set(+17, 0);
    LANDER_POLY[5].Set(+14, +17);

    landerShape_.Set(LANDER_POLY, 6);

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
    if (contactDetector == nullptr) {
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


LunarLander::reset() {
    destroy();
    contactDetector_ = new ContactDetector(this);
    world_.SetContactListener(contactDetector_);
    gameOver_ = false;
    prevShaping_ = 0;


    //
    int H = VIEWPORT_H/SCALE;
    int W = VIEWPORT_W/SCALE;

    // generate terrain
    int CHUNKS = 11;
    float height[CHUNK+1];
    float chunkX[CHUNK];

    // random sample a bunch of heights 0 to H/2
    std::uniform_real_distribution<> dis(0, H/2);
    for (int i = 0; i < 12; i++) {
        height[i] = dis(gen);
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


    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 4.0f);
    b2Body* body = world.CreateBody(&bodyDef);

    world.SetContactListener(contactListener);


    self.world.contactListener_keepref = ContactDetector(self)
    self.world.contactListener = self.world.contactListener_keepref
    self.game_over = False
    self.prev_shaping = None

    W = VIEWPORT_W/SCALE
    H = VIEWPORT_H/SCALE

    # terrain
    CHUNKS = 11
    height = self.np_random.uniform(0, H/2, size=(CHUNKS+1,) )
    chunk_x  = [W/(CHUNKS-1)*i for i in range(CHUNKS)]
    self.helipad_x1 = chunk_x[CHUNKS//2-1]
    self.helipad_x2 = chunk_x[CHUNKS//2+1]
    self.helipad_y  = H/4
    height[CHUNKS/2 - 2] = self.helipad_y
    height[CHUNKS/2 - 1] = self.helipad_y
    height[CHUNKS/2 + 0] = self.helipad_y
    height[CHUNKS/2 + 1] = self.helipad_y
    height[CHUNKS/2 + 2] = self.helipad_y
    smooth_y = [0.33*(height[i-1] + height[i+0] + height[i+1]) for i in range(CHUNKS)]

    self.moon = self.world.CreateStaticBody( shapes=edgeShape(vertices=[(0, 0), (W, 0)]) )
    self.sky_polys = []
    for i in range(CHUNKS-1):
        p1 = (chunk_x[i],   smooth_y[i])
        p2 = (chunk_x[i+1], smooth_y[i+1])
        self.moon.CreateEdgeFixture(
            vertices=[p1,p2],
            density=0,
            friction=0.1)
        self.sky_polys.append( [p1, p2, (p2[0],H), (p1[0],H)] )

    self.moon.color1 = (0.0,0.0,0.0)
    self.moon.color2 = (0.0,0.0,0.0)

    initial_y = VIEWPORT_H/SCALE
    self.lander = self.world.CreateDynamicBody(
        position = (VIEWPORT_W/SCALE/2, initial_y),
        angle=0.0,
        fixtures = fixtureDef(
            shape=polygonShape(vertices=[ (x/SCALE,y/SCALE) for x,y in LANDER_POLY ]),
            density=5.0,
            friction=0.1,
            categoryBits=0x0010,
            maskBits=0x001,  # collide only with ground
            restitution=0.0) # 0.99 bouncy
            )
    self.lander.color1 = (0.5,0.4,0.9)
    self.lander.color2 = (0.3,0.3,0.5)
    self.lander.ApplyForceToCenter( (
        self.np_random.uniform(-INITIAL_RANDOM, INITIAL_RANDOM),
        self.np_random.uniform(-INITIAL_RANDOM, INITIAL_RANDOM)
        ), True)

    self.legs = []
    for i in [-1,+1]:
        leg = self.world.CreateDynamicBody(
            position = (VIEWPORT_W/SCALE/2 - i*LEG_AWAY/SCALE, initial_y),
            angle = (i*0.05),
            fixtures = fixtureDef(
                shape=polygonShape(box=(LEG_W/SCALE, LEG_H/SCALE)),
                density=1.0,
                restitution=0.0,
                categoryBits=0x0020,
                maskBits=0x001)
            )
        leg.ground_contact = False
        leg.color1 = (0.5,0.4,0.9)
        leg.color2 = (0.3,0.3,0.5)
        rjd = revoluteJointDef(
            bodyA=self.lander,
            bodyB=leg,
            localAnchorA=(0, 0),
            localAnchorB=(i*LEG_AWAY/SCALE, LEG_DOWN/SCALE),
            enableMotor=True,
            enableLimit=True,
            maxMotorTorque=LEG_SPRING_TORQUE,
            motorSpeed=+0.3*i  # low enough not to jump back into the sky
            )
        if i==-1:
            rjd.lowerAngle = +0.9 - 0.5  # Yes, the most esoteric numbers here, angles legs have freedom to travel within
            rjd.upperAngle = +0.9
        else:
            rjd.lowerAngle = -0.9
            rjd.upperAngle = -0.9 + 0.5
        leg.joint = self.world.CreateJoint(rjd)
        self.legs.append(leg)

    self.drawlist = [self.lander] + self.legs

return self._step(np.array([0,0]) if self.continuous else 0)[0]



}

LunarLander::render() {

}

LunarLander::step() {


}
