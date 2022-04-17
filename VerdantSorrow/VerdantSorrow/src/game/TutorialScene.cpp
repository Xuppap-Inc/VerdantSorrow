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
#include "../components/nivelHuida/Mushroom.h"

void TutorialScene::init() 
{

	Scene::init();
	//Para gestionar las colisiones
	colManager_ = new CollisionManager();


	background();

	//Se crea el jugador 
	auto player = mngr_->addEntity();
	playerGenerator(colManager_, player);

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
	fly->addToGroup(ecs::_BOSSELEMENTS_GRP);
}
void TutorialScene::createPlatform(int x, int y, int w, int h)
{
	auto platform = mngr_->addEntity();
	auto platformTr = platform->addComponent<Transform>();
	platformTr->init(Vector2D(x, y), Vector2D(), w, y, 0.0f);
	auto col = platform->addComponent<RectangleCollider>(platformTr->getWidth(), platformTr->getHeight());
	colManager_->addCollider(col);
	platform->addComponent<RectangleRenderer>();
	platform->addComponent<Mushroom>(colManager_);
	platform->addToGroup(ecs::_BOSSELEMENTS_GRP);
}
