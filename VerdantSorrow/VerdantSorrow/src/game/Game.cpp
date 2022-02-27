#include "Game.h"

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
#include "../components/FrogBoss/BossAtributos.h"
#include "../components/FrogBoss/FrogJump.h"
#include "../components/FrogBoss/FrogBigJump.h"
#include "../components/BossHPBar.h"
#include "../components/player/PlayerComponents.h"
#include "../components/Wave/WaveMovement.h"

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

	//Se crea el jugador 
	auto player = mngr_->addEntity();
	playerGenerator(colManager, player);
	frogGenerator(colManager, player);
	platformGenerator(colManager);
	//waveGenerator(colManager, player, sdlutils().width() / 2, -1);
	//waveGenerator(colManager, player, sdlutils().width() / 2, 1);
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


void Game::frogGenerator(CollisionManager* colManager, Entity* player_) {

	//Se crea a la rana 
	auto Frog = mngr_->addEntity();
	//Se añaden los atributos del boss que están junto al transform

	auto FrogAtribs = Frog->addComponent<BossAtributos>(3.0f);

	

	auto FrogTr = Frog->addComponent<Transform>();
	auto FrogX = sdlutils().width() / 2 - 25;
	auto FrogY = sdlutils().height();
	//Se le dan las posiciones iniciales, velocidad, ancho y alto a la rana
	FrogTr->init(Vector2D(FrogX, FrogY), Vector2D(), 250, 150, 0.0f);
	Frog->addComponent<FramedImage>(&sdlutils().images().at("ranajump"), 6, 6, 2000, 31);
	//Frog->addComponent<FramedImage>(&sdlutils().images().at("ranaidle"), 6, 4,150,24);

	//Se le añade un color inicial a la rana, en este caso es negro
	//Frog->addComponent<RectangleRenderer>(SDL_Color());

	//Se añade un collider a la rana
	auto frogCollider = Frog->addComponent<RectangleCollider>(FrogTr->getWidth(), FrogTr->getHeight());
	frogCollider->setIsTrigger(true);
	//Se añade el collider al colliderGameManager
	colManager->addCollider(frogCollider);
	//Collider de paredes
	Frog->addComponent<CollideWithBordersBoss>();
	Frog->addComponent<SimpleGravity>(1.5);
	Frog->addComponent<FrogJump>(30);
	//Frog->addComponent<FrogBigJump>(40);
	Frog->addComponent<BossHPBar>();

}

void Game::playerGenerator(CollisionManager* colManager, Entity* player_) {
	//Se le añaden los atributos del player, no los del transform
	player_->addComponent<PlayerAttributes>();
	//Se le añade el transform
	auto playerTr = player_->addComponent<Transform>();
	auto playerX = sdlutils().width() / 2 - 25;
	auto playerY = sdlutils().height() / 2 - 25;
	//Se le dan las posiciones iniciales, vecocidad, ancho y alto al player
	playerTr->init(Vector2D(playerX, playerY), Vector2D(), 100, 200, 0.0f);
	//Se le da un renderer rectangular blanco por defecto al player
	//player->addComponent<RectangleRenderer>();

	//IMPORTANTE: Ponerlo antes de CollideWithBorders siempre porque si no no se colisiona correctamente contra el suelo
	player_->addComponent<SimpleGravity>(2.0);

	//IMPORTANTE: Ponerlo antes del PlayerCtrl siempre porque si no se salta 2 veces
	player_->addComponent<CollideWithBorders>();
	//Se añade un collider al jugador
	auto playerCollider = player_->addComponent<RectangleCollider>(playerTr->getWidth(), playerTr->getHeight());
	colManager->addCollider(playerCollider);
	//Componente que permite controlar al jugador
	player_->addComponent<PlayerCtrl>(23, 8, 0.85, 4);

	//No poner estas físicas detrás del playerctrl, se hunde y no funciona el salto
	player_->addComponent<SimplePhysicsPlayer>(colManager);
	player_->addComponent<Image>(&sdlutils().images().at("chica"));

	//Componente de ataque del jugador
	auto playerAttackCollider = player_->addComponent<Attack>(50, 50, colManager);
	colManager->addCollider(playerAttackCollider);
	playerAttackCollider->setIsTrigger(true);

	//Componente ui jugador
	player_->addComponent<PlayerUI>(&sdlutils().images().at("tennis_ball"));
}
void Game::platformGenerator(CollisionManager* colManager) {

	//Se crea una plataforma de ejemplo
	auto platform = mngr_->addEntity();
	//Se añade el transform a la plataforma
	auto platformTr = platform->addComponent<Transform>();
	auto platformX = sdlutils().width() / 3 - 200;
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
void Game::waveGenerator(CollisionManager* colManager, Entity* player_, float x, int dir) {

	//Se crea la onda expansiva
	auto Wave = mngr_->addEntity();
	//Se añaden los atributos del boss que están junto al transform
	auto WaveAtribs = Wave->addComponent<BossAtributos>();
	auto WaveTr = Wave->addComponent<Transform>();
	auto WaveX = x;
	auto WaveY = sdlutils().height() - 50;
	//dir = {-1, 1}
	auto WaveDir = dir;
	auto WaveSpeed = 5;
	//Se le dan las posiciones iniciales, velocidad, ancho y alto a la onda
	WaveTr->init(Vector2D(WaveX, WaveY), Vector2D(), 150, 50, 0.0f);
	//Se le añade un color inicial a la onda
	Wave->addComponent<RectangleRenderer>(SDL_Color());

	//Se añade un collider a la onda
	auto waveCollider = Wave->addComponent<RectangleCollider>(WaveTr->getWidth(), WaveTr->getHeight());
	waveCollider->setIsTrigger(true);
	//Se añade el collider al colliderGameManager
	colManager->addCollider(waveCollider);
	//Se añade el movimiento horizontal
	Wave->addComponent<WaveMovement>(WaveDir, WaveSpeed);
}