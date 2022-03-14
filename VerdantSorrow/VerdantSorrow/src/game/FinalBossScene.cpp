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

using ecs::Entity;
using ecs::Manager;

FinalBossScene::FinalBossScene() : mngr_(nullptr)
{
}

FinalBossScene::~FinalBossScene()
{
	delete mngr_;
}

void FinalBossScene::init()
{
	//SDLUtils::init("Verdant Sorrow", 1280, 720, "resources/config/resources.json");

	//Para gestionar las colisiones
	CollisionManager* colManager = new CollisionManager();
	mngr_ = new Manager();
	mngr_->setDebug(true); //activamos modo debug

	//background();
	//Se crea el jugador 
	auto player = mngr_->addEntity();
	playerGenerator(colManager, player);
	finalBossGenerator(colManager, player);
	//platformGenerator(colManager);
	//waveGenerator(colManager, player, sdlutils().width() / 2, -1);
	//waveGenerator(colManager, player, sdlutils().width() / 2, 1);
	//rootGenerator(colManager, player, sdlutils().width() / 2);
	//flyGenerator(colManager, player);
}

void FinalBossScene::start() {

	// a boolean to exit the loop
	bool exit = false;

	auto& ihdlr = ih();

	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();

		// refresh the input handler
		ihdlr.refresh();

		if (ihdlr.isKeyDown(SDL_SCANCODE_ESCAPE)) {
			exit = true;
			continue;
		}

		mngr_->update();
		mngr_->refresh();

		sdlutils().clearRenderer();
		mngr_->render();
		mngr_->debug();
		sdlutils().presentRenderer();

		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
	}

	SDL_Quit();
}

void FinalBossScene::background()
{
	auto backgr_ = mngr_->addEntity();
	auto backgr_Tr = backgr_->addComponent<Transform>();
	auto FrogX = 0;
	auto FrogY = 0;
	backgr_Tr->init(Vector2D(FrogX, FrogY), Vector2D(), sdlutils().width(), sdlutils().height(), 0.0f);
	backgr_->addComponent<Image>(&sdlutils().images().at("fondo1"));
}


void FinalBossScene::finalBossGenerator(CollisionManager* colManager, Entity* player_) {

	auto FinalBossFace = mngr_->addEntity();
	auto FinalBossAtribs = FinalBossFace->addComponent<BossAtributos>(10.0f);

	auto BossTr = FinalBossFace->addComponent<Transform>();
	auto BossX = (sdlutils().width()-100) / 2 ;
	auto BossY = sdlutils().height()/2;
	BossTr->init(Vector2D(BossX, BossY), Vector2D(2,2),100, 100, 0.0f);

	
	FinalBossFace->addComponent<Image>(&sdlutils().images().at("ojo"));
	FinalBossFace->addComponent<HandsManager>(colManager);
	FinalBossFace->addComponent<FinalBossMovement>();
	//Se añade un collider a la rana
	auto bossCollider = FinalBossFace->addComponent<RectangleCollider>(BossTr->getWidth(), BossTr->getHeight());
	bossCollider->setIsTrigger(true);
	colManager->addCollider(bossCollider);

	//FinalBossFace->addComponent<SimpleGravity>(1.5);
	//FinalBossFace->addComponent<CollideWithBordersBoss>();

	FinalBossFace->addComponent<BossHPBar>();





}



void FinalBossScene::playerGenerator(CollisionManager* colManager, Entity* player_) {
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
	player_->addComponent<CollideWithBorders>();

	//Se añade un collider al jugador
	auto playerCollider = player_->addComponent<RectangleCollider>(playerTr->getWidth() - 40, playerTr->getHeight()-30);
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
	player_->addComponent<PlayerUI>();
	mngr_->setHandler(ecs::_PLAYER, player_);

	// Animacion del jugador
	//player_->addComponent<FramedImage>(&sdlutils().images().at("ranajump"), 6, 6, 2000, 31);
}


void FinalBossScene::waveGenerator(CollisionManager* colManager, Entity* player_, float x, int dir) {

	//Se crea la onda expansiva
	auto Wave = mngr_->addEntity();
	//Se añaden los atributos del boss que están junto al transform
	auto WaveAtribs = Wave->addComponent<BossAtributos>();
	auto WaveTr = Wave->addComponent<Transform>();
	auto WaveX = x;
	auto WaveY = sdlutils().height() - 50;
	//dir = {-1, 1}
	auto WaveDir = dir;
	auto WaveSpeed = 5;
	//Se le dan las posiciones iniciales, velocidad, ancho y alto a la onda
	WaveTr->init(Vector2D(WaveX, WaveY), Vector2D(), 150, 50, 0.0f);
	//Se le añade un color inicial a la onda
	Wave->addComponent<RectangleRenderer>(SDL_Color());

	//Se añade un collider a la onda
	auto waveCollider = Wave->addComponent<RectangleCollider>(WaveTr->getWidth(), WaveTr->getHeight());
	waveCollider->setIsTrigger(true);
	//Se añade el collider al colliderGameManager
	colManager->addCollider(waveCollider);
	//Se añade el movimiento horizontal
	//Wave->addComponent<WaveMovement>(WaveDir, WaveSpeed);
}