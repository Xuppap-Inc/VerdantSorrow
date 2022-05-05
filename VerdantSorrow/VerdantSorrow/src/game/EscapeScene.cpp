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
#include "../components/boss/finalBoss/EscapeSceneBoss.h"

#include "../components/boss/wave/WaveSpawner.h"

#include "SceneManager.h"
#include "../game/CollisionChecker.h"
#include "../components/fondos/ParticleSystem.h"
#include "TileMap.h"

#include "../components/boss/finalBoss/FinalBossMovement.h"

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

	//Creación del boss 
	createBoss(colManager);

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
	cameraC->setLimitsToPositions(tilemapTr->getWidth(), 0, tilemapTr->getPos().getY(), sdlutils().height());
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
	playerDying_ = false;
	mngr_->update();
	if(!playerDying_){
		colCheck_->checkCollisions();
		mngr_->refresh();
		sdlutils().clearRenderer();
		mngr_->render();
#ifdef _DEBUG
		mngr_->debug();
#endif
		sdlutils().presentRenderer();
		changescenes();
	}
	
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


void EscapeScene::createBoss(CollisionManager* col)
{
	FinalBossFace = mngr_->addEntity();
	mngr_->setHandler(ecs::_EYE, FinalBossFace);
	FinalBossFace->addToGroup(ecs::_BOSS_GRP);

	// poner a 30
	auto FinalBossAtribs = FinalBossFace->addComponent<BossAtributos>(30);

	auto BossTr = FinalBossFace->addComponent<Transform>();
	auto BossX = 0;
	auto BossY = sdlutils().height() / 2 ;
	BossTr->init(Vector2D(BossX, BossY), Vector2D(0, 0), 350, 200, 0.0f, 0.55f);


	FinalBossFace->addComponent<FramedImage>(&sdlutils().images().at("FinalBoss_Fase2"), 5, 4, 800, 20, "FinalBoss_Fase2");
	BossTr->setScale(.3);

	auto movement_ = FinalBossFace->addComponent<EscapeSceneBoss>(col);
	//movement_->setPhase(FinalBossMovement::Phase::PHASE2);

	float colliderWidth = 200;
	auto bossCollider = FinalBossFace->addComponent<RectangleCollider>(colliderWidth, colliderWidth);

	bossCollider->setIsTrigger(true);
	col->addCollider(bossCollider);
}

void EscapeScene::changescenes()
{
	if (player->getComponent<PlayerAttributes>()->getLives() <= 0) {
		init();
	}
}
