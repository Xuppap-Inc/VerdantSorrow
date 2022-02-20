#include "Game.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Collisions.h"

#include "../ecs/Manager.h"
#include "../ecs/Entity.h"

#include "../components/RectangleRenderer.h"
#include "../components/RectangleCollider.h"
#include "../components/Transform.h"
#include "../components/FrogBoss/BossAtributos.h"

#include "../components/player/PlayerComponents.h"

#include "../components/FrogBoss/CollideWithBordersBoss.h"
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

	playerGenerator(colManager);
	frogGenerator(colManager);
	platformGenerator(colManager);
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


void Game::frogGenerator(CollisionManager* colManager) {

	//Se crea a la rana 
	auto Frog = mngr_->addEntity();
	auto FrogTr = Frog->addComponent<BossAtributos>();
	auto FrogX = sdlutils().width() / 2 - 25;
	auto FrogY = sdlutils().height();
	FrogTr->init(Vector2D(FrogX, FrogY), Vector2D(), 50, 50, 0.0f, 3.0f);
	Frog->addComponent<RectangleRenderer>(SDL_Color());

	//Se a�ade un collider a la rana
	auto frogCollider = Frog->addComponent<RectangleCollider>(FrogTr->getWidth(), FrogTr->getHeight());
	colManager->addCollider(frogCollider);
	//Collider de paredes
	Frog->addComponent<CollideWithBordersBoss>();
}

void Game::playerGenerator(CollisionManager* colManager) {

	//Se crea el jugador 
	auto player = mngr_->addEntity();
	//Se le añaden los atributos del player, no los del transform
	player->addComponent<PlayerAttributes>();
	//Se le añade el transform
	auto playerTr = player->addComponent<Transform>();
	auto playerX = sdlutils().width() / 2 - 25;
	auto playerY = sdlutils().height() / 2 - 25;
	//Se le dan las posiciones iniciales, vecocidad, ancho y alto al player
	playerTr->init(Vector2D(playerX, playerY), Vector2D(), 50, 50, 0.0f);
	//Se le da un renderer rectangular blanco por defecto al player
	player->addComponent<RectangleRenderer>();

	//IMPORTANTE: Ponerlo antes de CollideWithBorders siempre porque si no no se colisiona correctamente contra el suelo
	player->addComponent<SimpleGravity>(2.0);

	//IMPORTANTE: Ponerlo antes del PlayerCtrl siempre porque si no se salta 2 veces
	player->addComponent<CollideWithBorders>();

	//Componente que permite controlar al jugador
	player->addComponent<PlayerCtrl>(23, 8);

	//Se añade un collider al jugador
	//new RectangleCollider(player->getWidth(), player->getHeight())
	auto playerCollider = player->addComponent<RectangleCollider>(playerTr->getWidth(), playerTr->getHeight());
	colManager->addCollider(playerCollider);
	player->addComponent<SimplePhysicsPlayer>(colManager);

	//Componente de ataque del jugador
	player->addComponent<Attack>(50, 50);
}
void Game::platformGenerator(CollisionManager* colManager) {

	//Se crea una plataforma de ejemplo
	auto platform = mngr_->addEntity();
	//Se añade el transform a la plataforma
	auto platformTr = platform->addComponent<Transform>();
	auto platformX = sdlutils().width() / 3;
	auto platformY = sdlutils().height() / 4 * 3;
	//Se le dan las posiciones iniciales, velocidad, ancho y alto a la plataforma
	platformTr->init(Vector2D(platformX, platformY), Vector2D(), 200, 50, 0.0f);
	//Se le da un renderer rectangular, blanco por defecto
	platform->addComponent<RectangleRenderer>();

	//Se crea un collider para la plataforma
	auto platformCollider = platform->addComponent<RectangleCollider>(platformTr->getWidth(), platformTr->getHeight());
	//Se añade el collider de la plataforma al colliderManager
	colManager->addCollider(platformCollider);
}
