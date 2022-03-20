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
#include "../components/boss/tree_boss/Root/RootMovement.h"
#include "CollisionManager.h"



void TutorialScene::init()
{

	Scene::init();
	//Para gestionar las colisiones
	CollisionManager* colManager = new CollisionManager();


	background();

	//Se crea el jugador 
	auto player = mngr_->addEntity();
	playerGenerator(colManager, player);

	auto particles = mngr_->addEntity();
	particles->addComponent<Transform>(Vector2D(0, 0), Vector2D(), sdlutils().width(), sdlutils().height(), 0.0f);
	particles->addComponent<FramedImage>(&sdlutils().images().at("particles"), 14, 5, 2000, 32, "particles");

}

void TutorialScene::background()
{
	Scene::background("fondoSuelo");
}


void TutorialScene::playerGenerator(CollisionManager* colManager, Entity* player_) {
	player_->addComponent<PlayerAttributes>();

	auto playerTr = player_->addComponent<Transform>();
	auto playerX = 0;
	auto playerY = sdlutils().height() / 2 - 25;
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
	auto playerAttackCollider = player_->addComponent<Attack>(50, playerTr->getHeight(), colManager);
	colManager->addCollider(playerAttackCollider);
	playerAttackCollider->setIsTrigger(true);

	//Componente ui jugador
	//player_->addComponent<PlayerUI>(&sdlutils().images().at("heart"), &sdlutils().images().at("heartBlack"));
	player_->addComponent<PlayerUI>();
	mngr_->setHandler(ecs::_PLAYER, player_);

	// Animacion del jugador
	//player_->addComponent<FramedImage>(&sdlutils().images().at("ranajump"), 6, 6, 2000, 31);
}
