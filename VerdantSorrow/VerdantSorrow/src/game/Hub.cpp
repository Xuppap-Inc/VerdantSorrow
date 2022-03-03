#include "Hub.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Collisions.h"

#include "../ecs/Manager.h"
#include "../ecs/Entity.h"

#include "../components/RectangleRenderer.h"
#include "../components/RectangleCollider.h"
#include "../components/Transform.h"
#include "../components/FramedImage.h"
#include "../components/Image.h"
#include "../components/player/PlayerComponents.h"
#include "../components/player/PlayerHubControl.h"

#include "CollisionManager.h"

using ecs::Entity;
using ecs::Manager;

Hub::Hub() : mngr_(nullptr)
{
}

Hub::~Hub()
{
	delete mngr_;
}

void Hub::init()
{
	

	//Para gestionar las colisiones
	CollisionManager* colManager = new CollisionManager();
	mngr_ = new Manager();

	//Se crea el jugador 
	auto player = mngr_->addEntity();
	playerGenerator(colManager, player);
	EntryGenerator(colManager);
}

void Hub::start() {

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
	SDL_Quit();
}

void Hub::playerGenerator(CollisionManager* colManager, Entity* player_) {
	//Se le a�aden los atributos del player, no los del transform
	player_->addComponent<PlayerAttributes>();
	//Se le a�ade el transform
	auto playerTr = player_->addComponent<Transform>();
	auto playerX = sdlutils().width() / 2 - 25;
	auto playerY = sdlutils().height() / 2 - 25;
	//Se le dan las posiciones iniciales, vecocidad, ancho y alto al player
	playerTr->init(Vector2D(playerX, playerY), Vector2D(), 50, 100, 0.0f,false);
	
	//IMPORTANTE: Ponerlo antes del PlayerCtrl siempre porque si no se salta 2 veces
	//Se a�ade un collider al jugador
	auto playerCollider = player_->addComponent<RectangleCollider>(playerTr->getWidth(), playerTr->getHeight());
	player_->addComponent<CollideWithBorders>();
	colManager->addCollider(playerCollider);
	//Componente que permite controlar al jugador
	player_->addComponent<PlayerHubControl>(8,colManager);

	//No poner estas f�sicas detr�s del playerctrl, se hunde y no funciona el salto
	//player_->addComponent<SimplePhysicsPlayer>(colManager);
	player_->addComponent<Image>(&sdlutils().images().at("chica"));


	//Componente ui jugador
	player_->addComponent<PlayerUI>(&sdlutils().images().at("tennis_ball"));
}

void Hub::EntryGenerator(CollisionManager* colManager)
{
	auto frogEntry = mngr_->addEntity();

	auto frogEntryTr = frogEntry->addComponent<Transform>();
	auto frogEntryX = sdlutils().width() / 3 - 200;
	auto frogEntryY = sdlutils().height() / 4 * 3;
	frogEntryTr->init(Vector2D(frogEntryX, frogEntryY), Vector2D(), 200, 50, 0.0f);

	frogEntry->addComponent<RectangleRenderer>();

	auto frogEntryCollider = frogEntry->addComponent<RectangleCollider>(frogEntryTr->getWidth(), frogEntryTr->getHeight());
	colManager->addCollider(frogEntryCollider);
	frogEntryCollider->setIsTrigger(true);
}
