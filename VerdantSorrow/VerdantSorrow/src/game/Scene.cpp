#include "Scene.h"

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

Scene::Scene() : mngr_(nullptr)
{
}

Scene::~Scene()
{
	delete mngr_;
}

void Scene::init()
{
	mngr_ = new ecs::Manager();
#ifdef _DEBUG
	mngr_->setDebug(true);
#endif
	camera_ = mngr_->addEntity();
	auto camTr = camera_->addComponent<Transform>();
	mngr_->setHandler(ecs::_hdlr_CAMERA, camera_);
}

//void Scene::start()
//{
//	// a boolean to exit the loop
//	bool exit = false;
//
//	auto& ihdlr = ih();
//
//	while (!exit) {
//		Uint32 startTime = sdlutils().currRealTime();
//
//		// refresh the input handler
//		ihdlr.refresh();
//
//		if (ihdlr.isKeyDown(SDL_SCANCODE_ESCAPE)) {
//			exit = true;
//			continue;
//		}
//
//		mngr_->update();
//		mngr_->refresh();
//
//		sdlutils().clearRenderer();
//		mngr_->render();
//		mngr_->debug();
//		sdlutils().presentRenderer();
//
//		Uint32 frameTime = sdlutils().currRealTime() - startTime;
//
//		if (frameTime < 10)
//			SDL_Delay(10 - frameTime);
//	}
//
//	SDL_Quit();
//}

void Scene::update()
{
}

void Scene::backgroundmovement(std::string backgroundName)
{
	auto backgrs_ = mngr_->addEntity();
	auto backgrs_Tr = backgrs_->addComponent<Transform>(Vector2D(0, 0), Vector2D(), sdlutils().width(), sdlutils().height(), 0.0f);
	backgrs_->addComponent<FramedImage>(&sdlutils().images().at(backgroundName), 10, 8, 5000, 78, backgroundName);

	auto suelo = mngr_->addEntity();
	auto suelo_Tr = suelo->addComponent<Transform>(Vector2D(0, 0), Vector2D(), sdlutils().width(), sdlutils().height() / 5, 0.0f);
	suelo->addComponent<Image>(&sdlutils().images().at("Suelo"));
}

void Scene::background(std::string backgroundName, int height)
{
	auto backgr_ = mngr_->addEntity();
	auto backgr_Tr = backgr_->addComponent<Transform>(Vector2D(0, 0), Vector2D(), sdlutils().width(), sdlutils().height(), 0.0f);
	backgr_->addComponent<Image>(&sdlutils().images().at(backgroundName));

	auto suelo = mngr_->addEntity();
	auto suelo_Tr = suelo->addComponent<Transform>(Vector2D(0, sdlutils().height() - height), Vector2D(), sdlutils().width(), height, 0.0f);
	suelo->addComponent<Image>(&sdlutils().images().at("Suelo"));
	backgr_->addToGroup(ecs::_BACKGROUND_1_GRP);
	suelo->addToGroup(ecs::_BACKGROUND_1_GRP);
}

void Scene::playerGenerator(CollisionManager* colManager, Entity* player_)
{
	player_->addComponent<PlayerAttributes>();

	auto playerTr = player_->addComponent<Transform>();
	auto playerX = 0;
	auto playerY = sdlutils().height() / 2 - 25;
	//playerTr->init(Vector2D(playerX, playerY), Vector2D(),80, 160, 0.0f);
	playerTr->init(Vector2D(playerX, playerY), Vector2D(), 50, 140, 0.0f, 0.5f);

	//player_->addComponent<FramedImage>(&sdlutils().images().at("Chica_Idle"), 5, 7, 5000, 30);
	player_->addComponent<FramedImage>(&sdlutils().images().at("Chica_Idle"), 5, 6, 5000, 30, "Chica_Idle");

	//IMPORTANTE: Ponerlo antes de CollideWithBorders siempre
	player_->addComponent<SimpleGravity>(1);
	//IMPORTANTE: Ponerlo antes del PlayerCtrl siempre porque si no se salta 2 veces
	auto collide = player_->addComponent<CollideWithBorders>(100);
	collide->collisionx(false);
	//Se a�ade un collider al jugadordd
	auto playerCollider = player_->addComponent<RectangleCollider>(playerTr->getWidth(), playerTr->getHeight());
	colManager->addCollider(playerCollider);

	//IMPORTANTE :No poner estas f�sicas detr�s del playerctrl
	player_->addComponent<SimplePhysicsPlayer>(colManager);

	//player_->addComponent<Image>(&sdlutils().images().at("chica"));

	//Componente de ataque del jugador
	auto playerAttackCollider = player_->addComponent<Attack>(135, playerTr->getHeight() * 1.8, -playerTr->getHeight() * 1.5 / 3, colManager);
	colManager->addCollider(playerAttackCollider);
	playerAttackCollider->setIsTrigger(true);

	// float jumpForce, float speed, float deceleration, float rollSpeed
	player_->addComponent<PlayerCtrl>(15, 6, 0.7, 10);
	mngr_->setHandler(ecs::_PLAYER, player_);
	//Componente ui jugador
	//player_->addComponent<PlayerUI>(&sdlutils().images().at("heart"), &sdlutils().images().at("heartBlack"));

	auto playerLife_ = mngr_->addEntity();
	playerLife_->addComponent<PlayerUI>();
	playerLife_->addToGroup(ecs::_UI_GRP);


	// Animacion del jugador
	//player_->addComponent<FramedImage>(&sdlutils().images().at("ranajump"), 6, 6, 2000, 31);


	player_->addToGroup(ecs::_PLAYER_GRP);
}
