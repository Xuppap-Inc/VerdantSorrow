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
		colCheck_->checkCollisions();
		mngr_->refresh();

		sdlutils().clearRenderer();
		mngr_->render();
		mngr_->debug();
		sdlutils().presentRenderer();
	}
	else {
		if (bossHealth <= 0) setAble(false);
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
	//auto FrogTr = Frog->addComponent<Transform>(Vector2D(FrogX, FrogY), Vector2D(), frogW, frogH, 0.0f);
	auto FrogTr = Frog->addComponent<Transform>();

	FrogTr->init(Vector2D(FrogX, FrogY), Vector2D(), frogW, frogH, 0.0f, 1);

	Frog->addComponent<FramedImage>(&sdlutils().images().at("ranajump"), 6, 6, 5000, 32, "ranajump");
	//Frog->addComponent<FramedImage>(&sdlutils().images().at("ranaidle"), 6, 4,150,24);

	//Se añade un collider a la rana
	auto frogCollider = Frog->addComponent<RectangleCollider>(FrogTr->getWidth()-150, FrogTr->getHeight()-200);
	frogCollider->setIsTrigger(true);
	colManager->addCollider(frogCollider);
		
	Frog->addComponent<SimpleGravity>(.5);
	Frog->addComponent<CollideWithBordersBoss>();

	Frog->addComponent<FrogAttackManager>(colManager);
	////Frog->addComponent<FrogJump>(30);
	////Frog->addComponent<FrogBigJump>(40);

	Frog->addComponent<BossHPBar>();
}

bool FrogScene::getAble()
{
	return isAble;
}

void FrogScene::setAble(bool a)
{
	isAble = a;
}
