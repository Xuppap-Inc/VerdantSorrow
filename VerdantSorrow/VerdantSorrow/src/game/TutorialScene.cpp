#include "TutorialScene.h"

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
#include "../components/boss/BossComponents.h"
#include "CollisionManager.h"
#include "../components/tutorial/TutorialFly.h"
#include "../components/tutorial/TutorialSpawnRoot.h"

void TutorialScene::init() 
{

	Scene::init();
	//Para gestionar las colisiones
	colManager_ = new CollisionManager();


	background();

	//Se crea el jugador 
	auto player = mngr_->addEntity();
	playerGenerator(colManager_, player);

	auto particles = mngr_->addEntity();
	particles->addComponent<Transform>(Vector2D(0, 0), Vector2D(), sdlutils().width(), sdlutils().height(), 0.0f);
	particles->addComponent<FramedImage>(&sdlutils().images().at("particles"), 14, 5, 2000, 32, "particles");

	createFly(400, sdlutils().height() - 200);
	createFly(700, sdlutils().height() - 600);

	createPlatform(700, sdlutils().height() - 100, 300, 100);

	auto rootSpawner = mngr_->addEntity();
	rootSpawner->addComponent<TutorialSpawnRoot>(colManager_);
}

void TutorialScene::background()
{
	Scene::background("fondoSuelo");
}

void TutorialScene::update()
{
	mngr_->update();
	mngr_->refresh();

	sdlutils().clearRenderer();
	mngr_->render();
	mngr_->debug();
	sdlutils().presentRenderer();
}




void TutorialScene::playerGenerator(CollisionManager* colManager, Entity* player_) {
	player_->addComponent<PlayerAttributes>();

	auto playerTr = player_->addComponent<Transform>();
	auto playerX = 0;
	auto playerY = 150;
	//playerTr->init(Vector2D(playerX, playerY), Vector2D(),80, 160, 0.0f);
	playerTr->init(Vector2D(playerX, playerY), Vector2D(), 100, 200, 0.0f);

	//player_->addComponent<FramedImage>(&sdlutils().images().at("Chica_Idle"), 5, 7, 5000, 30);
	player_->addComponent<FramedImage>(&sdlutils().images().at("Chica_Idle"), 5, 6, 5000, 30, "Chica_Idle");

	//IMPORTANTE: Ponerlo antes de CollideWithBorders siempre
	player_->addComponent<SimpleGravity>(2.0);
	//IMPORTANTE: Ponerlo antes del PlayerCtrl siempre porque si no se salta 2 veces
	player_->addComponent<CollideWithBorders>(100);

	//Se añade un collider al jugador
	auto playerCollider = player_->addComponent<RectangleCollider>(playerTr->getWidth() - 30, playerTr->getHeight());
	colManager->addCollider(playerCollider);
	player_->addComponent<PlayerCtrl>(23, 8, 0.85, 12);

	//IMPORTANTE :No poner estas físicas detrás del playerctrl
	player_->addComponent<SimplePhysicsPlayer>(colManager);

	//player_->addComponent<Image>(&sdlutils().images().at("chica"));

	//Componente de ataque del jugador
	auto playerAttackCollider = player_->addComponent<Attack>(135, playerTr->getHeight() * 2, -playerTr->getHeight() * 1.5 / 3, colManager);
	colManager->addCollider(playerAttackCollider);
	playerAttackCollider->setIsTrigger(true);

	//Componente ui jugador
	//player_->addComponent<PlayerUI>(&sdlutils().images().at("heart"), &sdlutils().images().at("heartBlack"));
	player_->addComponent<PlayerUI>();
	mngr_->setHandler(ecs::_PLAYER, player_);

	// Animacion del jugador
	//player_->addComponent<FramedImage>(&sdlutils().images().at("ranajump"), 6, 6, 2000, 31);
}

void TutorialScene::createFly(int x, int y)
{
	auto fly = mngr_->addEntity();
	auto fTr = fly->addComponent<Transform>();
	auto flyX = x;
	auto flyY = y;
	fTr->init(Vector2D(flyX, flyY), Vector2D(), 60, 50, 0.0f);
	auto col = fly->addComponent<RectangleCollider>(fTr->getWidth(), fTr->getHeight());
	col->setIsTrigger(true);
	colManager_->addCollider(col);
	fly->addComponent<FramedImage>(&sdlutils().images().at("mosca"), 6, 6, (1000/30)*31, 31, "mosca");
	fly->addComponent<TutorialFly>();
}
void TutorialScene::createPlatform(int x, int y, int w, int h)
{
	auto platform = mngr_->addEntity();
	auto platformTr = platform->addComponent<Transform>();
	platformTr->init(Vector2D(x, y), Vector2D(), w, y, 0.0f);
	auto col = platform->addComponent<RectangleCollider>(platformTr->getWidth(), platformTr->getHeight());
	colManager_->addCollider(col);
	platform->addComponent<RectangleRenderer>();
}
