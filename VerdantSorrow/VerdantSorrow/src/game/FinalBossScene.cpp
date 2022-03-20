#include "FinalBossScene.h"

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
#include "../components/boss/finalBoss/HandsManager.h"
#include "../components/boss/finalBoss/FinalBossMovement.h"

#include "../components/boss/wave/WaveSpawner.h"



void FinalBossScene::init()
{
	//SDLUtils::init("Verdant Sorrow", 1280, 720, "resources/config/resources.json");
	Scene::init();
	//Para gestionar las colisiones
	CollisionManager* colManager = new CollisionManager();


	background();

	waveSpawnerGenerator(colManager);

	//Se crea el jugador 
	auto player = mngr_->addEntity();
	playerGenerator(colManager, player);

	finalBossGenerator(colManager, player);

}

void FinalBossScene::update()
{
	mngr_->update();
	mngr_->refresh();

	sdlutils().clearRenderer();
	mngr_->render();
	mngr_->debug();
	sdlutils().presentRenderer();
}

void FinalBossScene::waveSpawnerGenerator(CollisionManager*& colManager)
{
	//se crea wave spwner
	auto waveSp = mngr_->addEntity();
	waveSp->addComponent<WaveSpawner>(colManager);
	mngr_->setHandler(ecs::_WAVE_GENERATOR, waveSp);
}

void FinalBossScene::background()
{
	Scene::background("fondo1");
}


void FinalBossScene::finalBossGenerator(CollisionManager* colManager, Entity* player_) {

	auto FinalBossFace = mngr_->addEntity();
	auto FinalBossAtribs = FinalBossFace->addComponent<BossAtributos>(10.0f);

	auto BossTr = FinalBossFace->addComponent<Transform>();
	auto BossX = (sdlutils().width() - 100) / 2;
	auto BossY = sdlutils().height() / 2;
	BossTr->init(Vector2D(BossX, BossY), Vector2D(0, 0), 100, 100, 0.0f);


	FinalBossFace->addComponent<Image>(&sdlutils().images().at("ojo"));
	FinalBossFace->addComponent<HandsManager>(colManager);
	FinalBossFace->addComponent<FinalBossMovement>(colManager);
	//Se añade un collider a la rana
	auto bossCollider = FinalBossFace->addComponent<RectangleCollider>(BossTr->getWidth(), BossTr->getHeight());
	bossCollider->setIsTrigger(true);
	colManager->addCollider(bossCollider);


	FinalBossFace->addComponent<BossHPBar>();
}



void FinalBossScene::playerGenerator(CollisionManager* colManager, Entity* player_) {
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
	player_->addComponent<CollideWithBorders>();

	//Se añade un collider al jugador
	auto playerCollider = player_->addComponent<RectangleCollider>(playerTr->getWidth() - 40, playerTr->getHeight());
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
	player_->addComponent<PlayerUI>();
	mngr_->setHandler(ecs::_PLAYER, player_);

	// Animacion del jugador
	//player_->addComponent<FramedImage>(&sdlutils().images().at("ranajump"), 6, 6, 2000, 31);
}

bool FinalBossScene::getAble()
{
	return isAble;
}

void FinalBossScene::setAble(bool a)
{
	isAble = a;
}
