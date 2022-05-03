#include "EscapeScene.h"

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
#include "../components/ScrollCamera.h"
#include "Fondos/Parallax.h"
#include "CollisionManager.h"

#include "../components/boss/wave/WaveSpawner.h"

#include "SceneManager.h"
#include "../game/CollisionChecker.h"
#include "../components/fondos/ParticleSystem.h"
#include "../components/fondos/Light.h"
#include "TileMap.h"

EscapeScene::EscapeScene() :Scene(), isAble(false), musicVolume_(60)
{
}

EscapeScene::~EscapeScene()
{
	delete tileMap_;
}

void EscapeScene::init()
{
	Scene::init();

	//Para gestionar las colisiones
	CollisionManager* colManager = new CollisionManager();
	mngr_->setColManager(colManager);

	tileMap_ = new TileMap(mngr_, "resources/Huida/nivelHuida.tmx", colManager, 1.5, TileMap::Pivot::BOTTONLEFT);
	background();


	//Se crea el jugador 
	player = mngr_->addEntity();
	playerGeneratorEscape(colManager, player);

	auto tilemapTr = mngr_->getHandler(ecs::_hdlr_TILEMAP)->getComponent<Transform>();
	auto camera = mngr_->addEntity();
	auto cameraTr = camera->addComponent<Transform>();
	cameraTr->init(Vector2D(0, 0), Vector2D(0, 0), 0, 0, 0);
	auto cameraC = camera->addComponent<ScrollCamera>(8);
	cameraC->lock(false, true);
	cameraC->setLimitsToPositions(tilemapTr->getWidth(),0,tilemapTr->getPos().getY(),sdlutils().height());
	mngr_->setHandler(ecs::_hdlr_CAMERA, camera);

	colCheck_ = new CollisionChecker(colManager, mngr_);
	ParticleSystem* particlesys = new ParticleSystem(&sdlutils().images().at("particle"), mngr_);
	particlesys->createParticlesAsh(100);

	musica_ = &sdlutils().musics().at("music_huida");
	musica_->play();
	musica_->setMusicVolume(musicVolume_);
}

void EscapeScene::update()
{

	mngr_->update();
	colCheck_->checkCollisions();
	mngr_->refresh();
	changescenes();
	sdlutils().clearRenderer();
	mngr_->render();
#ifdef _DEBUG
	mngr_->debug();
#endif
	sdlutils().presentRenderer();
}



void EscapeScene::background()
{
	auto parallax_ = new Parallax(mngr_);

	parallax_->AddLayer(&sdlutils().images().at("Parallax_Layer5"), 1, sdlutils().height(), sdlutils().width(), 0);
	parallax_->AddLayer(&sdlutils().images().at("Parallax_Layer4"), 0.3f, sdlutils().height(), sdlutils().width(), 0);
	parallax_->AddLayer(&sdlutils().images().at("Parallax_Layer3"), 0.2f, sdlutils().height(), sdlutils().width(), 80);
	parallax_->AddLayer(&sdlutils().images().at("Parallax_Layer2"), 0.1f, sdlutils().height(), sdlutils().width(), 80);
	parallax_->AddLayer(&sdlutils().images().at("Parallax_Layer1"), 0, sdlutils().height(), sdlutils().width(), 80);
}


bool EscapeScene::getAble()
{
	return isAble;
}

void EscapeScene::setAble(bool a)
{
	isAble = a;
}

void EscapeScene::createLights() {
	new Light(&sdlutils().images().at("luz_rojo"), 200, 100, 100, 100, mngr_);

}

void EscapeScene::changescenes()
{
	if (player->getComponent<PlayerAttributes>()->getLives() <= 0) {
		init();
	}
}
