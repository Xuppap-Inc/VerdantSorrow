#include "Hub.h"

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
#include "../components/player/PlayerHubControl.h"
#include "../components/hub/NpcCtrl.h"
#include "../components/hub/DialogBoxMngr.h"
#include "../components/ScrollCamera.h"
#include "../components/CameraShake.h"

#include "CollisionManager.h"
#include "../game/SceneManager.h"
#include "../components/fondos/ParticleSystem.h"
#include "../components/fondos/Light.h"
#include "../components/hub/PlatformAtribsForHub.h"
#include "../sdlutils/Texture.h"
#include "../json/JSONValue.h"
#include "TileMap.h"
#include "FrogScene.h"
#include "../components/boss/frog_boss/FrogAttackManager.h"
#include "../components/FollowCamera.h"
#include "Game.h"




Hub::Hub() :Scene(), musicVolume_(60), firstTimeEnteringGame(true),
frogDoorPos({ 2500 + 1400, 1825 + 1400}),
treeDoorPos({ 2500 + 1400, 1825 - 700}),
finalDoorPos({ 2500 - 1400, 1825 - 500})
{
	colManager = nullptr;
}

Hub::~Hub()
{
	delete tileMap_;
}

void Hub::init()
{
	Scene::init();

	//Para gestionar las colisiones
	colManager = new CollisionManager();
	mngr_->setColManager(colManager);

	tileMap_ = new TileMap(mngr_, "resources/hub/mapa.tmx", colManager, 0.6);

	changeSc = false;

	playerGenerator(colManager);

	teleportPlayer();

	auto camera = mngr_->addEntity();
	auto cameraTr = camera->addComponent<Transform>();

	auto playerTr_ = player_->getComponent<Transform>();

	auto x = playerTr_->getPos().getX() - playerTr_->getWidth() / 2 - sdlutils().width() / 2;
	auto y = playerTr_->getPos().getY() - sdlutils().height() / 2;

	cameraTr->init(Vector2D(x, y), Vector2D(0, 0), 0, 0, 0);
	auto cameraC = camera->addComponent<ScrollCamera>(3);
	auto tilemapTR = mngr_->getHandler(ecs::_hdlr_TILEMAP)->getComponent<Transform>();
	cameraC->setLimitToDimensions(tilemapTR->getPos().getX(), tilemapTR->getPos().getY(), tilemapTR->getWidth(), tilemapTR->getHeight());
	mngr_->setHandler(ecs::_hdlr_CAMERA, camera);

	musica_ = &sdlutils().musicsHub().at("musica_hub");
	musica_->play();
	musica_->setMusicVolume(musicVolume_);
	musica_->resumeMusic();

	ParticleSystem* particlesys_ = new ParticleSystem(&sdlutils().imagesHub().at("particula_menu"), mngr_);
	particlesys_->createParticlesMenu(30);

	createLights();

	if (blackScreen_ == nullptr) {
		createBlackScreen();
		blackScreen_->addComponent<FollowCamera>();
	}

	else {
	
		delete blackScreen_;

		createBlackScreen();
		blackScreen_->addComponent<FollowCamera>();
	}

	blackScreenImg_->setAlpha(255);
	blackScreenImg_->fadeOut();
}

void Hub::teleportPlayer()
{
	auto tilemapTr = mngr_->getHandler(ecs::_hdlr_TILEMAP)->getComponent<Transform>();

	// SpawnpointInicial
	Vector2D spawnPoint = { tilemapTr->getWidth() / 2 - 200, tilemapTr->getHeight() / 2 };

	if (!firstTimeEnteringGame) {

		// Si el jugador a muerto y vuelve al hub, teletransportarlo a la puerta del boss en el que murio
		if (Game::instance()->playerJustKilled)
		{
			Game::instance()->playerJustKilled = false;

			switch (Game::instance()->state_)
			{
			case Game::State::HUB:
				// Aparecer en la entrada del jefe del rana
				spawnPoint = frogDoorPos;
				break;

			case Game::State::FROGDEFEATED:
				// Aparecer en la entrada del jefe del arbol
				spawnPoint = treeDoorPos;
				break;

			case Game::State::TREEDEFEATED:
				// Aparecer en la entrada del jefe final
				spawnPoint = finalDoorPos;
				break;

			default:
				break;
			}
		}
		// Si el jugador ha matado al jefe y vuelve al hub
		else {
			switch (Game::instance()->state_)
			{
			case Game::State::FROGDEFEATED:
				// Aparecer en la entrada del jefe del rana
				spawnPoint = frogDoorPos;
				break;

			case Game::State::TREEDEFEATED:
				// Aparecer en la entrada del jefe del arbol
				spawnPoint = treeDoorPos;
				break;

			case Game::State::FINALDEFEATED:
				// Aparecer en la entrada del jefe final
				spawnPoint = finalDoorPos;
				break;

			default:
				break;
			}
		}
	}
	else
		firstTimeEnteringGame = false;

	player_->getComponent<Transform>()->getPos().set(spawnPoint);
}



bool Hub::getAble()
{
	return isAble;
}

void Hub::setAble(bool a)
{
	isAble = a;
}

void Hub::changeScene_(bool ch)
{
	if (ch) {
	
		blackScreenImg_->setAlpha(0);
		blackScreenImg_->fadeIn(2);
	}

	changeSc = ch;
}

void Hub::checkCollissions()
{
	auto playerCol_ = player_->getComponent<RectangleCollider>();
	if (colManager->hasCollisions(playerCol_)) {
		std::vector<RectangleCollider*> colliders = colManager->getCollisions(playerCol_);
		bool changeScene = false;
		int i = 0;
		Entity* entWithName = new Entity();
		while (!changeScene && i < colliders.size()) {
			changeScene = colliders[i]->isActive() && colliders[i]->isTrigger() && colliders[i]->getEntity()->getComponent<NpcCtrl>() == nullptr;
			if (changeScene) entWithName = colliders[i]->getEntity();
			i++;
		}
		if (changeScene) {
			if (entWithName->getComponent<PlatformAtribsForHub>()->getName() == "frog") sC().FrogSceneState(true);
			else if (entWithName->getComponent<PlatformAtribsForHub>()->getName() == "tree") sC().TreeSceneState(true);
			else if (entWithName->getComponent<PlatformAtribsForHub>()->getName() == "eye") sC().EyeSceneState(true);
			else if (entWithName->getComponent<PlatformAtribsForHub>()->getName() == "end") sC().EyeSceneState(false);
			changeScene_(true);
		}
	}
}

void Hub::update()
{
	if (!changeSc) {
		mngr_->update();
		mngr_->refresh();

		sdlutils().clearRenderer();

		mngr_->render();
#ifdef _DEBUG
		mngr_->debug();
#endif
		sdlutils().presentRenderer();

		checkCollissions();
	}
	else if (blackScreenImg_->getAlpha() >= 255) {

		sC().decideScene();
	}

	else {
	
		blackScreen_->update();

		sdlutils().clearRenderer();
		mngr_->render();
		sdlutils().presentRenderer();
	}
}

void Hub::playerGenerator(CollisionManager* colManager) {
	//Se le a�aden los atributos del player, no los del transform
	player_ = mngr_->addEntity();

	player_->addComponent<PlayerAttributes>();

	//Se le a�ade el transform
	auto playerTr = player_->addComponent<Transform>();
	auto playerX = sdlutils().width() / 2;
	cout << sdlutils().width() / 2 << endl;
	auto playerY = sdlutils().height() / 2;
	playerTr->init(Vector2D(playerX, playerY), Vector2D(), 70, 150, 0.0f, 0.25f, false);
	auto s = player_->addComponent<FramedImage>(&sdlutils().imagesHub().at("idle_Kyna"), 4, 8, (1000 / 30) * 30, 30, "idle_Kyna");


	//IMPORTANTE: Ponerlo antes del PlayerCtrl siempre porque si no se salta 2 veces
	//Se a�ade un collider al jugador
	playerTr->setScale(0.15);


	auto playerCollider = player_->addComponent<RectangleCollider>(playerTr->getWidth(), playerTr->getHeight());
	colManager->addCollider(playerCollider);
	auto collision_ = player_->addComponent<SimplePhysicsPlayer>(colManager);
	collision_->gravedad(false);


	//Componente que permite controlar al jugador
	player_->addComponent<PlayerHubControl>(2, colManager);


	//Componente ui jugador
	mngr_->setHandler(ecs::_PLAYER, player_);
	auto* playerLife_ = mngr_->addEntity();
	playerLife_->addComponent<PlayerUI>(true);
	playerLife_->addToGroup(ecs::_UI_GRP);



	player_->addToGroup(ecs::_HUB_DECORATION_GRP);
}

void Hub::createLights() {
	new Light(&sdlutils().imagesHub().at("luz_naranja"), -200, -100, 800, 100, mngr_);
	
}