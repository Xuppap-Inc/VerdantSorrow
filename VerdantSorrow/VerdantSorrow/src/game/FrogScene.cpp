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
#include "../components/boss/tree_boss/Root/RootMovement.h"


#include "CollisionManager.h"




void FrogScene::init()
{
	
	Scene::init();
	//Para gestionar las colisiones
	CollisionManager* colManager = new CollisionManager();

	
	background();
	//Se crea el jugador 
	auto player = mngr_->addEntity();
	playerGenerator(colManager, player);
	frogGenerator(colManager, player);
	//platformGenerator(colManager);
	//waveGenerator(colManager, player, sdlutils().width() / 2, -1);
	//waveGenerator(colManager, player, sdlutils().width() / 2, 1);
	//rootGenerator(colManager, player, sdlutils().width() / 2);
	//flyGenerator(colManager, player);
}


void FrogScene::background()
{
	Scene::background("fondo1");
}


void FrogScene::frogGenerator(CollisionManager* colManager, Entity* player_) {

	auto Frog = mngr_->addEntity();
	auto FrogAtribs = Frog->addComponent<BossAtributos>(10.0f);

	auto FrogX = sdlutils().width() / 2 - 25;
	auto FrogY = sdlutils().height();
	auto FrogTr = Frog->addComponent<Transform>(Vector2D(FrogX, FrogY), Vector2D(), 250*2, 150*2, 0.0f);


	Frog->addComponent<FramedImage>(&sdlutils().images().at("ranajump"), 6, 6, 5000, 32, "ranajump");
	//Frog->addComponent<FramedImage>(&sdlutils().images().at("ranaidle"), 6, 4,150,24);
	
	//Se añade un collider a la rana
	auto frogCollider = Frog->addComponent<RectangleCollider>(FrogTr->getWidth()-150, FrogTr->getHeight());
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
	playerTr->init(Vector2D(playerX, playerY), Vector2D(),100, 200, 0.0f);

	//player_->addComponent<FramedImage>(&sdlutils().images().at("Chica_Idle"), 5, 7, 5000, 30);
	player_->addComponent<FramedImage>(&sdlutils().images().at("Chica_Idle"), 5, 6, 5000, 30, "Chica_Idle");

	//IMPORTANTE: Ponerlo antes de CollideWithBorders siempre
	player_->addComponent<SimpleGravity>(2.0);
	//IMPORTANTE: Ponerlo antes del PlayerCtrl siempre porque si no se salta 2 veces
	player_->addComponent<CollideWithBorders>();

	//Se añade un collider al jugador
	auto playerCollider = player_->addComponent<RectangleCollider>(playerTr->getWidth() - 30, playerTr->getHeight());
	colManager->addCollider(playerCollider);
	player_->addComponent<PlayerCtrl>(23, 8, 0.85, 12);

	//IMPORTANTE :No poner estas físicas detrás del playerctrl
	player_->addComponent<SimplePhysicsPlayer>(colManager);

	//player_->addComponent<Image>(&sdlutils().images().at("chica"));

	//Componente de ataque del jugador
	auto playerAttackCollider = player_->addComponent<Attack>(50,playerTr->getHeight(), colManager);
	colManager->addCollider(playerAttackCollider);
	playerAttackCollider->setIsTrigger(true);

	//Componente ui jugador
	//player_->addComponent<PlayerUI>(&sdlutils().images().at("heart"), &sdlutils().images().at("heartBlack"));
	player_->addComponent<PlayerUI>();
	mngr_->setHandler(ecs::_PLAYER, player_);

	// Animacion del jugador
	//player_->addComponent<FramedImage>(&sdlutils().images().at("ranajump"), 6, 6, 2000, 31);
}
void FrogScene::flyGenerator(CollisionManager* colManager, Entity* player_) {


	auto Fly = mngr_->addEntity();
	auto FlyAtribs = Fly->addComponent<BossAtributos>(1.0f);
	auto FlyTr = Fly->addComponent<Transform>();
	auto playerTr = player_->getComponent<Transform>();
	auto FlyX = playerTr->getPos().getX();
	auto distY= 100;
	auto FlyY = playerTr->getPos().getY();

	//Se le dan las posiciones iniciales, velocidad, ancho y alto a la rana
	FlyTr->init(Vector2D(FlyX, FlyY+distY), Vector2D(), 50,50, 0.0f);

	//Se le añade un color inicial en este caso es negro
	Fly->addComponent<RectangleRenderer>(SDL_Color());

	//Componente que se usa para seguir al jugador
	//Fly->addComponent<FollowPlayer>(playerTr, 150.0f,-100.0f, Vector2D(), 7.0f);
}

void FrogScene::platformGenerator(CollisionManager* colManager) {

	auto platform = mngr_->addEntity();

	auto platformTr = platform->addComponent<Transform>();
	auto platformX = sdlutils().width() / 3 - 200;
	auto platformY = sdlutils().height() / 4 * 3;
	platformTr->init(Vector2D(platformX, platformY), Vector2D(), 200, 50, 0.0f);

	platform->addComponent<RectangleRenderer>();

	auto platformCollider = platform->addComponent<RectangleCollider>(platformTr->getWidth(), platformTr->getHeight());
	colManager->addCollider(platformCollider);
}
void FrogScene::waveGenerator(CollisionManager* colManager, Entity* player_, float x, int dir) {

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
	Wave->addComponent<WaveMovement>(WaveDir, WaveSpeed);
}