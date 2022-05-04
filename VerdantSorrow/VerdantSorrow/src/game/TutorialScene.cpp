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
#include "CollisionChecker.h"

void TutorialScene::init() 
{

	Scene::init();
	//Para gestionar las colisiones
	colManager_ = new CollisionManager();
	mngr_->setColManager(colManager_);
	colCheck_ = new CollisionChecker(colManager_, mngr_);
	changeScene_ = false;
	background();

	//Se crea el jugador 
	auto player = mngr_->addEntity();
	playerGenerator(colManager_, player);

	createFly(2500 * 0.6, sdlutils().height() - 700 * 0.6);
	createPlatform(2200 * 0.6, sdlutils().height() - 500 * 0.6,
		300 * 0.6, 100 * 0.6);
	createPlatform(1700 * 0.6, sdlutils().height() - 800 * 0.6,
		300 * 0.6, 100 * 0.6);
	createPlatform(1500 * 0.6, sdlutils().height() - 1100 * 0.6,
		300 * 0.6, 100 * 0.6);
	createPlatform(1200 * 0.6, sdlutils().height() - 1150 * 0.6,
		300 * 0.6, 100 * 0.6);
	createDoor(1200 * 0.6, sdlutils().height() - 1150 * 0.6 - 400 * 0.6,
		300 * 0.6, 400 * 0.6);

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
	colCheck_->checkCollisions();
	if (!changeScene_) {
		mngr_->refresh();

		sdlutils().clearRenderer();
		mngr_->render();
#ifdef _DEBUG
		mngr_->debug();
#endif
		sdlutils().presentRenderer();
	}
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
	platformTr->init(Vector2D(x, y), Vector2D(), w, h, 0.0f);
	auto col = platform->addComponent<RectangleCollider>(platformTr->getWidth(), platformTr->getHeight());
	colManager_->addCollider(col);
	platform->addComponent<Image>(&sdlutils().images().at("platform"));
	platform->addToGroup(ecs::_BOSSELEMENTS_GRP);
}
void TutorialScene::createDoor(int x, int y, int w, int h)
{
	auto door = mngr_->addEntity();
	auto doorTr = door->addComponent<Transform>();
	doorTr->init(Vector2D(x, y), Vector2D(), w, h, 0.0f);
	auto col = door->addComponent<RectangleCollider>(doorTr->getWidth(), doorTr->getHeight());
	colManager_->addCollider(col);
	col->setIsTrigger(true); 
	col->setActive(false);
	auto doorIm =door->addComponent<Image>(&sdlutils().images().at("door"));
	doorIm->setVisible(false);
	mngr_->setHandler(ecs::_hdlr_TUTORIALENTRY, door);
	door->addToGroup(ecs::_BOSSELEMENTS_GRP);
}


