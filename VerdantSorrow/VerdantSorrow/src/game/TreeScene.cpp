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
#include "../components/boss/tree_boss/MeleeAttack.h"
#include "../components/boss/tree_boss/TreeMovement.h"
#include "../components/boss/tree_boss/LanternSpawner.h"
#include "../components//boss//tree_boss/LanternMovement.h"
#include "../components//boss//tree_boss/LanternCollisions.h"


#include "CollisionManager.h"
#include "SceneManager.h"
#include "../game/CollisionChecker.h"


void TreeScene::init()
{
	//SDLUtils::init("Verdant Sorrow", 1280, 720, "resources/config/resources.json");
	Scene::init();
	//Para gestionar las colisiones
	CollisionManager* colManager = new CollisionManager();


	//Se crea el jugador 
	player = mngr_->addEntity();
	playerGenerator(colManager);
	treeGenerator(colManager);

	colCheck_ = new CollisionChecker(colManager, mngr_);
}


void TreeScene::update()
{
	auto health = player->getComponent<PlayerAttributes>()->getLives();
	auto bossHealth = tree_->getComponent<BossAtributos>()->getLife();
	if (health > 0 && bossHealth > 0) {
		mngr_->update();
	/*	colCheck_->collisionsGrootScene();*/
		mngr_->refresh();

		sdlutils().clearRenderer();
		mngr_->render();
		mngr_->debug();
		sdlutils().presentRenderer();
	}
	else {
		sC().decideScene();
	}
}

void TreeScene::treeGenerator(CollisionManager* colManager) {

	tree_ = mngr_->addEntity();

	mngr_->setHandler(ecs::_TREEBOSS, tree_);
	tree_->addComponent<BossAtributos>(10.0f);
	auto treeTr = tree_->addComponent<Transform>();
	auto treeX = sdlutils().width() / 4 * 3 - 80;
	auto treeY = sdlutils().height() - 360;
	treeTr->init(Vector2D(treeX, treeY), Vector2D(), 160, 360, 0.0f);

	lanternGenerator(colManager, tree_, treeTr->getPos().getX(), treeTr->getPos().getY());

	tree_->addComponent<Image>(&sdlutils().images().at("groot"));

	//Se añade un collider al arbol
	auto treeCollider = tree_->addComponent<RectangleCollider>(treeTr->getWidth(), treeTr->getHeight());
	treeCollider->setIsTrigger(true);
	colManager->addCollider(treeCollider);

	//IMPORTANTE: movimiento y spawner antes de los ataques
	tree_->addComponent<TreeMovement>(player->getComponent<Transform>(), 2.0f);
	tree_->addComponent<RootSpawner>(colManager);

	tree_->addComponent<RootWave>();
	tree_->addComponent<RootAutoAim>(player);
	tree_->addComponent<MeleeAttack>(50, player->getComponent<Transform>()->getHeight(), colManager);

	//IMPORTANTE: attack manager al final
	tree_->addComponent<TreeAttackManager>(colManager);
}

void TreeScene::playerGenerator(CollisionManager* colManager) {
	player->addComponent<PlayerAttributes>();

	auto playerTr = player->addComponent<Transform>();
	auto playerX = sdlutils().width() / 4 - 25;
	auto playerY = sdlutils().height() / 2 - 25;
	playerTr->init(Vector2D(playerX, playerY), Vector2D(), 100, 200, 0.0f);


	player->addComponent<FramedImage>(&sdlutils().images().at("Chica_Idle"), 5, 7, 5000, 30, "Chica_Idle");
	//IMPORTANTE: Ponerlo antes de CollideWithBorders siempre 
	player->addComponent<SimpleGravity>(2.0);
	//IMPORTANTE: Ponerlo antes del PlayerCtrl siempre porque si no se salta 2 veces
	player->addComponent<CollideWithBorders>();

	//Se añade un collider al jugador
	auto playerCollider = player->addComponent<RectangleCollider>(playerTr->getWidth() - 16, playerTr->getHeight());
	colManager->addCollider(playerCollider);
	player->addComponent<PlayerCtrl>(23, 8, 0.85, 12);

	//IMPORTANTE :No poner estas físicas detrás del playerctrl
	player->addComponent<SimplePhysicsPlayer>(colManager);

	//Componente de ataque del jugador
	auto playerAttackCollider = player->addComponent<Attack>(50, playerTr->getHeight(), colManager);
	colManager->addCollider(playerAttackCollider);
	playerAttackCollider->setIsTrigger(true);

	//Componente ui jugador
	player->addComponent<PlayerUI>();
	mngr_->setHandler(ecs::_PLAYER, player);
}

void TreeScene::rootGenerator(CollisionManager* colManager, float x) {

	//Se crea la raiz
	auto Root = mngr_->addEntity();
	//Se añaden los atributos del boss que están junto al transform
	auto RootAtribs = Root->addComponent<BossAtributos>();
	auto RootTr = Root->addComponent<Transform>();
	auto RootX = x;
	auto RootY = sdlutils().height() + 200;
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

void TreeScene::lanternGenerator(CollisionManager* colManager, Entity* tree_, float x, float y)
{
	//distribucion random de intervalo variable en distintas llamadas
	std::random_device seed;
	std::mt19937 gen{ seed() };
	std::uniform_int_distribution <>myrand(0, sdlutils().width());

	//crea entidad linterna
	auto lantern = mngr_->addEntity();

	mngr_->setHandler(ecs::_LANTERN, lantern);

	//atributos de linterna
	auto lanternAtribs = lantern->addComponent<BossAtributos>(10.0f);
	lantern->addComponent<BossHPBar>();
	auto lanternTr = lantern->addComponent<Transform>();
	auto lanternX = x;
	auto lanternY = y;

	//damos paramtros iniciales
	lanternTr->init(Vector2D(lanternX, lanternY), Vector2D(), 100, 100, 0.0f);
	//de momento con un color pocho
	lantern->addComponent<Image>(&sdlutils().images().at("lampara"));

	//collider de la lampara
	auto lanternCollider = lantern->addComponent <RectangleCollider>(lanternTr->getWidth(), lanternTr->getHeight());
	//se hace trigger
	lanternCollider->setIsTrigger(true);
	//le pasamos el collider al manager
	colManager->addCollider(lanternCollider);
	//la damos movimiento
	lantern->addComponent<LanternMovement>();

	lantern->addComponent<LanternCollisions>(colManager);
}

bool TreeScene::getAble()
{
	return isAble;
}

void TreeScene::setAble(bool a)
{
	isAble = a;
}
