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

#include "SceneManager.h"
#include "../game/CollisionChecker.h"


void FinalBossScene::init()
{
	//SDLUtils::init("Verdant Sorrow", 1280, 720, "resources/config/resources.json");
	Scene::init();
	//Para gestionar las colisiones
	CollisionManager* colManager = new CollisionManager();


	background();

	waveSpawnerGenerator(colManager);

	//Se crea el jugador 
	player = mngr_->addEntity();
	playerGenerator(colManager, player);

	finalBossGenerator(colManager, player);

	colCheck_ = new CollisionChecker(colManager, mngr_);
}

void FinalBossScene::update()
{
	auto health = player->getComponent<PlayerAttributes>()->getLives();
	auto bossHealth = FinalBossFace->getComponent<BossAtributos>()->getLife();
	if (health > 0 && bossHealth > 0) {
		mngr_->update();
		colCheck_->checkCollisions();
		mngr_->refresh();

		sdlutils().clearRenderer();
		mngr_->render();
		mngr_->debug();
		sdlutils().presentRenderer();
	}
	else {
		sC().decideScene();
	}
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

	FinalBossFace = mngr_->addEntity();
	auto FinalBossAtribs = FinalBossFace->addComponent<BossAtributos>(10.0f);

	auto BossTr = FinalBossFace->addComponent<Transform>();
	auto BossX = (sdlutils().width() - 350) / 2;
	auto BossY = sdlutils().height() / 2 - 200;
	BossTr->init(Vector2D(BossX, BossY), Vector2D(0, 0), 350, 200, 0.0f);


	FinalBossFace->addComponent<Image>(&sdlutils().images().at("ojo"));
	FinalBossFace->addComponent<HandsManager>(colManager);
	FinalBossFace->addComponent<FinalBossMovement>(colManager);

	auto bossCollider = FinalBossFace->addComponent<RectangleCollider>(BossTr->getWidth() - 50, BossTr->getHeight() - 50);
	bossCollider->setIsTrigger(true);
	colManager->addCollider(bossCollider);

	FinalBossFace->addComponent<BossHPBar>();
}

bool FinalBossScene::getAble()
{
	return isAble;
}

void FinalBossScene::setAble(bool a)
{
	isAble = a;
}
