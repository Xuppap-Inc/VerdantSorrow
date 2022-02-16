#include "Game.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Collisions.h"

#include "../ecs/Manager.h"
#include "../ecs/Entity.h"
#include "../components/RectangleRenderer.h"
#include "../components/PlayerCtrl.h"
#include "../components/SimplePhysicsPlayer.h"
#include "../components/player/PlayerAttributes.h"
#include "../components/CollideWithBorders.h"
#include "../components/RectangleCollider.h"
#include "../components/Transform.h"

#include "CollisionManager.h"

using ecs::Entity;
using ecs::Manager;

Game::Game() : mngr_(nullptr)
{
}

Game::~Game()
{
	delete mngr_;
}

void Game::init()
{
	SDLUtils::init("Verdant Sorrow", 1280, 720, "resources/config/resources.json");

	//Para gestionar las colisiones
	CollisionManager* colManager = new CollisionManager();
	mngr_ = new Manager();

	//Se crea el jugador 
	auto player = mngr_->addEntity();
	player->addComponent<PlayerAttributes>();

	auto playerTr = player->addComponent<Transform>();
	auto playerX = sdlutils().width() / 2 - 25;
	auto playerY = sdlutils().height() / 2 - 25;
	playerTr->init(Vector2D(playerX, playerY), Vector2D(), 50,50,0.0f);	
	player->addComponent<RectangleRenderer>();

	player->addComponent<PlayerCtrl>(20, 8);
	//Componente que permite controlar al jugador
	//PlayerCtrl* playerController = new PlayerCtrl(30, 8);
	//player->addComponent(playerController);

	//Se añade un collider al jugador
	//new RectangleCollider(player->getWidth(), player->getHeight())
	auto playerCollider = player->addComponent<RectangleCollider>(playerTr->getWidth(), playerTr->getHeight());
	colManager->addCollider(playerCollider);
	//player->addComponent(playerCollider);
	player->addComponent<SimplePhysicsPlayer>(1.0, colManager);
	player->addComponent<CollideWithBorders>();


	//Se crea una plataforma de ejemplo
	auto platform = mngr_->addEntity();
	auto platformTr = platform->addComponent<Transform>();
	auto platformX = sdlutils().width() / 3;
	auto platformY = sdlutils().height() / 4 * 3;
	platformTr->init(Vector2D(platformX, platformY), Vector2D(), 200, 50, 0.0f);
	platform->addComponent<RectangleRenderer>();
	
	//Se crea un collider para la plataforma
	auto platformCollider = platform->addComponent<RectangleCollider>(platformTr->getWidth(), platformTr->getHeight());
	colManager->addCollider(platformCollider);
}

void Game::start() {

	// a boolean to exit the loop
	bool exit = false;

	auto& ihdlr = ih();

	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();

		// refresh the input handler
		ihdlr.refresh();

		if (ihdlr.isKeyDown(SDL_SCANCODE_ESCAPE)) {
			exit = true;
			continue;
		}

		mngr_->update();
		mngr_->refresh();

		sdlutils().clearRenderer();
		mngr_->render();
		sdlutils().presentRenderer();

		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
	}

}