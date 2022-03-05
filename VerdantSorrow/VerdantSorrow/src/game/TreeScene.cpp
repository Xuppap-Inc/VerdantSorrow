#include "TreeScene.h"

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
#include "../components/boss/tree_boss/TreeAttackManager.h"
#include "../components/boss/tree_boss/RootWave.h"
#include "../components/boss/tree_boss/Root/RootMovement.h"
#include "../components/boss/tree_boss/RootSpawner.h"
#include "../components/boss/tree_boss/RootAutoAim.h"


#include "CollisionManager.h"

using ecs::Entity;
using ecs::Manager;

TreeScene::TreeScene() : mngr_(nullptr)
{
}

TreeScene::~TreeScene()
{
	delete mngr_;
}

void TreeScene::init()
{
	//SDLUtils::init("Verdant Sorrow", 1280, 720, "resources/config/resources.json");

	//Para gestionar las colisiones
	CollisionManager* colManager = new CollisionManager();
	mngr_ = new Manager();
	mngr_->setDebug(true);

	//Se crea el jugador 
	auto player = mngr_->addEntity();
	playerGenerator(colManager, player);
	treeGenerator(colManager, player);
}

void TreeScene::start() {

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


void TreeScene::treeGenerator(CollisionManager* colManager, Entity* player_) {

	auto Tree_ = mngr_->addEntity();
	auto TreeAtribs = Tree_->addComponent<BossAtributos>(3.0f);

	auto TreeTr = Tree_->addComponent<Transform>();
	auto TreeX = sdlutils().width() / 2 - 80;
	auto TreeY = sdlutils().height() - 320;
	TreeTr->init(Vector2D(TreeX, TreeY), Vector2D(), 160, 320, 0.0f);

	Tree_->addComponent<Image>(&sdlutils().images().at("chica"));

	//Se añade un collider al arbol
	auto treeCollider = Tree_->addComponent<RectangleCollider>(TreeTr->getWidth(), TreeTr->getHeight());
	treeCollider->setIsTrigger(true);
	colManager->addCollider(treeCollider);

	Tree_->addComponent<BossHPBar>();

	Tree_->addComponent<RootSpawner>(colManager);
	Tree_->addComponent<RootWave>();
	Tree_->addComponent<RootAutoAim>(player_);
	Tree_->addComponent<TreeAttackManager>(colManager);

}

void TreeScene::playerGenerator(CollisionManager* colManager, Entity* player_) {
	player_->addComponent<PlayerAttributes>();

	auto playerTr = player_->addComponent<Transform>();
	auto playerX = sdlutils().width() / 2 - 25;
	auto playerY = sdlutils().height() / 2 - 25;
	playerTr->init(Vector2D(playerX, playerY), Vector2D(),80, 160, 0.0f);

	//IMPORTANTE: Ponerlo antes de CollideWithBorders siempre 
	player_->addComponent<SimpleGravity>(2.0);
	//IMPORTANTE: Ponerlo antes del PlayerCtrl siempre porque si no se salta 2 veces
	player_->addComponent<CollideWithBorders>();

	//Se añade un collider al jugador
	auto playerCollider = player_->addComponent<RectangleCollider>(playerTr->getWidth() - 16, playerTr->getHeight());
	colManager->addCollider(playerCollider);
	player_->addComponent<PlayerCtrl>(23, 8, 0.85, 4);

	//IMPORTANTE :No poner estas físicas detrás del playerctrl
	player_->addComponent<SimplePhysicsPlayer>(colManager);

	player_->addComponent<Image>(&sdlutils().images().at("chica"));

	//Componente de ataque del jugador
	auto playerAttackCollider = player_->addComponent<Attack>(50,playerTr->getHeight(), colManager);
	colManager->addCollider(playerAttackCollider);
	playerAttackCollider->setIsTrigger(true);

	//Componente ui jugador
	player_->addComponent<PlayerUI>(&sdlutils().images().at("tennis_ball"));
	mngr_->setHandler(ecs::_PLAYER, player_);
}

void TreeScene::rootGenerator(CollisionManager* colManager, Entity* player_, float x) {

	//Se crea la raiz
	auto Root = mngr_->addEntity();
	//Se añaden los atributos del boss que están junto al transform
	auto RootAtribs = Root->addComponent<BossAtributos>();
	auto RootTr = Root->addComponent<Transform>();
	auto RootX = x;
	auto RootY = sdlutils().height()+200;
	//Se le dan las posiciones iniciales, velocidad, ancho y alto a la raiz
	RootTr->init(Vector2D(RootX, RootY), Vector2D(), 25, 1000, 0.0f);
	//Se le añade un color inicial a la raiz
	Root->addComponent<RectangleRenderer>(SDL_Color());

	//Se añade un collider a la onda
	auto RootCollider = Root->addComponent<RectangleCollider>(RootTr->getWidth(), RootTr->getHeight());
	RootCollider->setIsTrigger(true);
	//Se añade el collider al colliderGameManager
	colManager->addCollider(RootCollider);
	//Se añade el movimiento horizontal
	Root->addComponent<RootMovement>();
}