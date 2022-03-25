#include "FrogScene.h"

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
#include "../components/boss/wave/WaveMovement.h"
#include "../components/boss/frog_boss/FrogAttackManager.h"


#include "CollisionManager.h"
#include "../components/boss/wave/WaveSpawner.h"
#include "SceneManager.h"
#include "../game/CollisionChecker.h"


void FrogScene::init()
{
	Scene::init();
	//Para gestionar las colisiones
	CollisionManager* colManager = new CollisionManager();

	background();

	waveSpawerGenerator(colManager);

	//Se crea el jugador
	player = mngr_->addEntity();
	playerGenerator(colManager, player);

	frogGenerator(colManager, player);
	auto particles = mngr_->addEntity();
	particles->addComponent<Transform>(Vector2D(0, 0), Vector2D(), sdlutils().width(), sdlutils().height(), 0.0f);
	particles->addComponent<FramedImage>(&sdlutils().images().at("particles"), 14, 5, 2000, 32, "particles");

	colCheck_ = new CollisionChecker(colManager, mngr_);
}

void FrogScene::update()
{
	auto health = 0;
	auto bossHealth = 0;
	if(player!=nullptr)
	 health = player->getComponent<PlayerAttributes>()->getLives();
	if(Frog!=nullptr)
	 bossHealth = Frog->getComponent<BossAtributos>()->getLife();
	if (health > 0 && bossHealth > 0) {
		mngr_->update();
		colCheck_->collisionsFrogScene();
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

void FrogScene::waveSpawerGenerator(CollisionManager*& colManager)
{
	//se crea wave spwner
	auto waveSp = mngr_->addEntity();
	waveSp->addComponent<WaveSpawner>(colManager);
	mngr_->setHandler(ecs::_WAVE_GENERATOR, waveSp);
}

void FrogScene::background()
{
	Scene::backgroundmovement("fondos1");
}

void FrogScene::frogGenerator(CollisionManager* colManager, Entity* player_) {

	Frog = mngr_->addEntity();
	mngr_->setHandler(ecs::_FROGBOSS, Frog);
	auto FrogAtribs = Frog->addComponent<BossAtributos>(10.0f);

	auto frogH = 300;
	auto frogW = frogH * 1.11f;
	auto FrogX = sdlutils().width() / 2 - 25;
	auto FrogY = sdlutils().height() - frogH;
	auto FrogTr = Frog->addComponent<Transform>(Vector2D(FrogX, FrogY), Vector2D(), frogW, frogH, 0.0f);


	Frog->addComponent<FramedImage>(&sdlutils().images().at("ranajump"), 6, 6, 5000, 32, "ranajump");
	//Frog->addComponent<FramedImage>(&sdlutils().images().at("ranaidle"), 6, 4,150,24);

	//Se añade un collider a la rana
	auto frogCollider = Frog->addComponent<RectangleCollider>(FrogTr->getWidth(), FrogTr->getHeight()-100);
	frogCollider->setIsTrigger(true);
	colManager->addCollider(frogCollider);

	Frog->addComponent<SimpleGravity>(1.5);
	Frog->addComponent<CollideWithBordersBoss>();

	Frog->addComponent<FrogAttackManager>(colManager);
	////Frog->addComponent<FrogJump>(30);
	////Frog->addComponent<FrogBigJump>(40);

	Frog->addComponent<BossHPBar>();
}

void FrogScene::playerGenerator(CollisionManager* colManager, Entity* player_) {
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

bool FrogScene::getAble()
{
	return isAble;
}

void FrogScene::setAble(bool a)
{
	isAble = a;
}
