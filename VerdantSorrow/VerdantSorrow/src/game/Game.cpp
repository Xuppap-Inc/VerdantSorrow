#include "Game.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Collisions.h"

#include "Container.h"
#include "GameManager.h"
#include "RectangleRenderer.h"
#include "PlayerCtrl.h"
#include "box2d.h"

#include "box2d.h"

Game::Game() : player(nullptr)
{
}

Game::~Game()
{
	for (Container* o : objs_) {
		delete o;
	}
}

void Game::init()
{
	SDLUtils::init("Verdant Sorrow", 800, 600, "resources/config/resources.json");

	player = new Container();
	player->getPos().set(sdlutils().width() / 2 - 25, sdlutils().height() / 2 - 25);
	player->setHeight(50);
	player->setWidth(50);
	player->addComponent(new RectangleRenderer());
	player->addComponent(new PlayerCtrl(5, 5));

	objs_.push_back(player);


}

void Game::start()
{
	// a boolean to exit the loop
	bool exit = false;

	auto& ihdlr = ih();


	b2World world(b2Vec2(0.0f, 20.0f));

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, sdlutils().height()-objs_[0]->getHeight());
	b2Body* groundBody = world.CreateBody(&groundBodyDef);
	b2PolygonShape groundBox;
	groundBox.SetAsBox(sdlutils().width(), 0.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);



	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(sdlutils().width()/2, 4.0f);
	b2Body* body = world.CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f, 1.0f);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	body->CreateFixture(&fixtureDef);


	float timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();

		// handle input
		ihdlr.refresh();

		if (ihdlr.isKeyDown(SDL_SCANCODE_ESCAPE)) {
			exit = true;
			continue;
		}

		for (auto& o : objs_) {
			o->handleInput();
		}

		world.Step(timeStep, velocityIterations, positionIterations);

		b2Vec2 pos = body->GetPosition();
		objs_[0]->getPos().set(pos.x, pos.y);

		// update
		for (auto& o : objs_) {
			o->update();
		}

		sdlutils().clearRenderer();

		// render
		for (auto& o : objs_) {
			o->render();
		}

		sdlutils().presentRenderer();
		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 20)
			SDL_Delay(20 - frameTime);
	}
}
