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
#include "../components/fondos/Light.h"
#include "Game.h"
#include <iostream>
#include <fstream>


void FinalBossScene::init()
{
	Scene::init();
	playerDying = false;
	//Para gestionar las colisiones
	CollisionManager* colManager = new CollisionManager();
	mngr_->setColManager(colManager);

	background();

	waveSpawnerGenerator(colManager);

	//Se crea el jugador 
	player = mngr_->addEntity();
	playerGenerator(colManager, player);

	finalBossGenerator(colManager, player);

	colCheck_ = new CollisionChecker(colManager, mngr_);


	auto height = (sdlutils().height() / 5) + 40;


	auto suelo = mngr_->addEntity();
	auto suelo_Tr = suelo->addComponent<Transform>(Vector2D(0, sdlutils().height() - height), Vector2D(), sdlutils().width(), height, 0.0f);
	suelo->addComponent<Image>(&sdlutils().images().at("fondodelante"));
	suelo->addToGroup(ecs::_FIRST_GRP);

	playerDeathTimer_ = mngr_->addTimer();

	if (blackScreen_ == nullptr) createBlackScreen();

	blackScreenImg_->setAlpha(255);
	blackScreenImg_->fadeOut(3);
}

void FinalBossScene::update()
{
	auto bossHealth = FinalBossFace->getComponent<BossAtributos>();
	if (!playerAttribs_->isDefeated() && !bossHealth->isDefeated()) {
		mngr_->update();
		colCheck_->checkCollisions();
		mngr_->refresh();

		sdlutils().clearRenderer();
		mngr_->render();
#ifdef _DEBUG
		mngr_->debug();
#endif
		sdlutils().presentRenderer();

		if (playerDying) checkPlayerFinishedDying();
		else checkPlayerDied();
	}
	else {
		if (playerAttribs_->isDefeated()) sC().changeEyeEssenceState(true);
		if (bossHealth->isDefeated()) {
			sC().changeStatePlayerInBoss(false);
			Game::instance()->state_ = Game::State::FINALDEFEATED;

			ofstream myfile("resources/config/guardado.txt");
			if (myfile.is_open())
			{
				myfile << Game::State::FINALDEFEATED;
				myfile.close();
			}
			else cout << "No se puede abrir el guardado.txt";
		}
		setAble(false);
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
	mngr_->setHandler(ecs::_EYE, FinalBossFace);
	FinalBossFace->addToGroup(ecs::_BOSS_GRP);

	// poner a 30
	auto FinalBossAtribs = FinalBossFace->addComponent<BossAtributos>(30);

	auto BossTr = FinalBossFace->addComponent<Transform>();
	auto BossX = (sdlutils().width() - 350) / 2;
	auto BossY = sdlutils().height() / 2 - 250;
	BossTr->init(Vector2D(BossX, BossY), Vector2D(0, 0), 350, 200, 0.0f, 0.55f);

	FinalBossFace->addComponent<FramedImage>(&sdlutils().images().at("FinalBoss_Fase1"), 10, 6, 2000, 60, "FinalBoss_Fase1");
	BossTr->setScale(1);

	movement_ = FinalBossFace->addComponent<FinalBossMovement>(colManager);
	handsMngr_ = FinalBossFace->addComponent<HandsManager>(colManager);

	float colliderWidth = 200;
	auto bossCollider = FinalBossFace->addComponent<RectangleCollider>(colliderWidth, colliderWidth);

	bossCollider->setIsTrigger(true);
	colManager->addCollider(bossCollider);

	auto finalBossHPBar = mngr_->addEntity();
	finalBossHPBar->addComponent<BossHPBar>();
	finalBossHPBar->addToGroup(ecs::_UI_GRP);
}

void FinalBossScene::deactivateBoss()
{
	handsMngr_->deactivateBoss();
	movement_->setActive(false);
	playerCtrl_->setActive(false);
	playerDying = true;
}

bool FinalBossScene::getAble()
{
	return isAble;
}

void FinalBossScene::setAble(bool a)
{
	isAble = a;
}

