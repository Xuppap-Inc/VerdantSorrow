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
#include "../components/fondos/ParticleSystem.h"


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
	ParticleSystem* particlesys = new ParticleSystem(&sdlutils().images().at("particle"), 100, mngr_);
	particlesys->createParticles();

	auto height = (sdlutils().height() / 5) + 40;

	auto suelo = mngr_->addEntity();
	auto suelo_Tr = suelo->addComponent<Transform>(Vector2D(0, sdlutils().height() - height), Vector2D(), sdlutils().width(), height, 0.0f);
	suelo->addComponent<Image>(&sdlutils().images().at("fondodelante"));
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
		if (health > 0) setAble(false);
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
	Scene::background("fondoNegro", sdlutils().height() / 5);
}


void FinalBossScene::finalBossGenerator(CollisionManager* colManager, Entity* player_) {

	FinalBossFace = mngr_->addEntity();
	auto FinalBossAtribs = FinalBossFace->addComponent<BossAtributos>(20);

	auto BossTr = FinalBossFace->addComponent<Transform>();
	auto BossX = (sdlutils().width() - 350) / 2;
	auto BossY = sdlutils().height() / 2 - 250;
	BossTr->init(Vector2D(BossX, BossY), Vector2D(0, 0), 350, 200, 0.0f, 0.55f);

	//BossTr->setScale(.33);
	//FinalBossFace->addComponent<Image>(&sdlutils().images().at("ojo"));
	FinalBossFace->addComponent<FramedImage>(&sdlutils().images().at("FinalBoss_Fase1"), 10, 6, 2000, 60, "FinalBoss_Fase1");
	//FinalBossFace->addComponent<FramedImage>(&sdlutils().images().at("FinalBoss_Fase2"), 5, 4, 800, 20, "FinalBoss_Fase2");
	//anim_->changeanim(&sdlutils().images().at("FinalBoss_Fase2"), 4, 5, 800, 20, "FinalBoss_Fase2");

	FinalBossFace->addComponent<HandsManager>(colManager);
	FinalBossFace->addComponent<FinalBossMovement>(colManager);

	float colliderWidth = 100, colliderOffset_y = 100;
	auto bossCollider = FinalBossFace->addComponent<RectangleCollider>
		(colliderWidth, colliderWidth);

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
