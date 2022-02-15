#include "Game.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Collisions.h"

#include "Container.h"
#include "GameManager.h"
#include "RectangleRenderer.h"
#include "PlayerCtrl.h"
#include "SimplePhysicsPlayer.h"
#include "SimpleMove.h"
#include "CollideWithBorders.h"
#include "RectangleCollider.h"

#include "CollisionManager.h"

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
	SDLUtils::init("Verdant Sorrow", 1280, 720, "resources/config/resources.json");

	//Para gestionar las colisiones
	CollisionManager* colManager = new CollisionManager();

	//Se crea el jugador 
	player = new Container();
	player->getPos().set(sdlutils().width() / 2 - 25, sdlutils().height() / 2 - 25);
	player->setHeight(50);
	player->setWidth(50);
	player->addComponent(new RectangleRenderer());

	//Componente que permite controlar al jugador
	PlayerCtrl* playerController = new PlayerCtrl(30, 8);
	player->addComponent(playerController);
	player->addComponent(new SimpleMove());

	//Se añade un collider al jugador
	RectangleCollider* playerCollider = new RectangleCollider(player->getWidth(), player->getHeight());
	colManager->addCollider(playerCollider);
	player->addComponent(playerCollider);
	player->addComponent(new SimplePhysicsPlayer(2.5, colManager, playerCollider));
	player->addComponent(new CollideWithBorders(playerController));

	objs_.push_back(player);

	//Se crea una plataforma de ejemplo
	Container* platform = new Container();
	platform->getPos().set(sdlutils().width() / 3, sdlutils().height() / 4 * 3);
	platform->setHeight(50);
	platform->setWidth(200);
	platform->addComponent(new RectangleRenderer());
	
	//Se crea un collider para la plataforma
	RectangleCollider* platformCollider = new RectangleCollider(platform->getWidth(), platform->getHeight());
	colManager->addCollider(platformCollider);
	platform->addComponent(platformCollider);

	objs_.push_back(platform);
}

void Game::start()
{
	// a boolean to exit the loop
	bool exit = false;

	auto& ihdlr = ih();

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
