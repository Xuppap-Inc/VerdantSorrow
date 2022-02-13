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

Game::Game() : player(nullptr),player2(nullptr),mundo(b2Vec2(0.0f, 200.0f))
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
	

	


	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(sdlutils().width() / 2, 4.0f);
	body = mundo.CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f, 1.0f);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	body->CreateFixture(&fixtureDef);

	b2BodyDef bodyDef2;
	bodyDef2.type = b2_dynamicBody;
	bodyDef2.position.Set(sdlutils().width() / 2, 4.0f);
	body2 = mundo.CreateBody(&bodyDef2);
	b2PolygonShape dynamicBox2;
	dynamicBox.SetAsBox(1.0f, 1.0f);
	b2FixtureDef fixtureDef2;
	fixtureDef2.shape = &dynamicBox2;
	fixtureDef2.density = 1.0f;
	fixtureDef2.friction = 0.0f;
	body2->CreateFixture(&fixtureDef);


    player2 = new Container();
	player2->getPos().set(sdlutils().width() / 3 - 25, sdlutils().height() / 2 - 25);
	player2->setHeight(100);
	player2->setWidth(150);
	player2->addComponent(new RectangleRenderer());

	player = new Container();
	player->getPos().set(sdlutils().width() / 2 - 25, sdlutils().height() / 2 - 25);
	player->setHeight(50);
	player->setWidth(50);
	player->addComponent(new RectangleRenderer());
	player->addComponent(new PlayerCtrl(5, 5,body));

	objs_.push_back(player);
	objs_.push_back(player2);


}

void Game::start()
{
	// a boolean to exit the loop
	bool exit = false;

	auto& ihdlr = ih();


	


	float timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;
	



	//create ground
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, sdlutils().height() - objs_[0]->getHeight());
	b2Body* groundBody = mundo.CreateBody(&groundBodyDef);
	b2PolygonShape groundBox;
	groundBox.SetAsBox(sdlutils().width(), 0.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);

	//	bodyDef.type – b2Body.b2_staticBody;


	//fixDef.shape.SetAsBox(10, 0.5);

	//world.CreateBody(bodyDef).CreateFixture(fixDef);

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

		mundo.Step(timeStep, velocityIterations, positionIterations);

		
		
		b2Vec2 pos = body->GetPosition();
		//body->ApplyLinearImpulse(pos1, pos1, true);
		objs_[0]->getPos().set(pos.x, pos.y);
		b2Vec2 pos1 = body2->GetPosition();
		//body->ApplyLinearImpulse(pos1, pos1, true);
		objs_[1]->getPos().set(pos1.x, pos1.y);
	
		//body->ApplyAngularImpulse(b2Vec2(s.getX(), s.getY());
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
