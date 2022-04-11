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
#include "../components/fondos/ParticleSystem.h"
#include "../components/fondos/Light.h"


void TreeScene::init()
{
	//SDLUtils::init("Verdant Sorrow", 1280, 720, "resources/config/resources.json");
	Scene::init();
	//Para gestionar las colisiones
	CollisionManager* colManager = new CollisionManager();

	Scene::background("fondo2");
	//Se crea el jugador 
	player = mngr_->addEntity();
	playerGenerator(colManager, player);

	treeGenerator(colManager);

	colCheck_ = new CollisionChecker(colManager, mngr_);

	createLights();
}


void TreeScene::update()
{
	auto health = player->getComponent<PlayerAttributes>()->getLives();
	auto bossHealth = mngr_->getHandler(ecs::_LANTERN)->getComponent<BossAtributos>()->getLife();
	if (health > 0 && bossHealth > 0) {
		mngr_->update();
		colCheck_->checkCollisions();
		mngr_->refresh();

		sdlutils().clearRenderer();
		mngr_->render();
		mngr_->debug();
		sdlutils().presentRenderer();
	}
	else {
		if (health <= 0) sC().changeTreeEssenceState(true);
		if(bossHealth<=0)sC().changeStatePlayerInBoss(false);
			
		setAble(false);
		sC().decideScene();
	}
}

void TreeScene::treeGenerator(CollisionManager* colManager) {

	tree_ = mngr_->addEntity();

	mngr_->setHandler(ecs::_TREEBOSS, tree_);
	tree_->addComponent<BossAtributos>(100);
	auto treeTr = tree_->addComponent<Transform>();
	auto treeH = 360;
	auto treeW = treeH * 1.105;

	//suponemos resolucion nativa (1920x1080p)
	auto treeX = 1920 / 4 * 3 - 80;
	auto treeY = 1080 - treeH;
	treeTr->init(Vector2D(treeX, treeY), Vector2D(), treeW, treeH, 0.0f);

	lanternGenerator(colManager, tree_, treeTr->getPos().getX(), treeTr->getPos().getY());

	tree_->addComponent<FramedImage>(&sdlutils().images().at("arbol_capa_idle"), 5, 6, (1000 / 30) * 25, 25, "arbol_capa_idle");

	//Se añade un collider al arbol
	auto treeCollider = tree_->addComponent<RectangleCollider>(treeTr->getWidth()/2, treeTr->getHeight());
	treeCollider->setIsTrigger(true);
	colManager->addCollider(treeCollider);

	//IMPORTANTE: movimiento y spawner antes de los ataques
	tree_->addComponent<TreeMovement>(player->getComponent<Transform>(), 2.0f);
	tree_->addComponent<RootSpawner>(colManager);

	tree_->addComponent<RootWave>();
	tree_->addComponent<RootAutoAim>(player);
	tree_->addComponent<MeleeAttack>(100, treeH, colManager);

	//IMPORTANTE: attack manager al final
	tree_->addComponent<TreeAttackManager>(colManager);
	ParticleSystem* particlesys = new ParticleSystem(&sdlutils().images().at("particle"), 100, mngr_);
	particlesys->createParticles();
	tree_->addToGroup(ecs::_BOSS_GRP);
}

void TreeScene::rootGenerator(CollisionManager* colManager, float x) {

	//Se crea la raiz
	auto Root = mngr_->addEntity();
	//Se añaden los atributos del boss que están junto al transform
	auto RootAtribs = Root->addComponent<BossAtributos>();
	auto RootTr = Root->addComponent<Transform>();
	auto RootX = x;
	auto RootY = 1080 + 200;
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
	Root->addToGroup(ecs::_BOSS_GRP);
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
	auto lanternAtribs = lantern->addComponent<BossAtributos>(30);
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
	lantern->addToGroup(ecs::_BOSSELEMENTS_GRP);
	auto lanternHPBar = mngr_->addEntity();
	lanternHPBar->addComponent<BossHPBar>();
	lanternHPBar->addToGroup(ecs::_UI_GRP);
}

bool TreeScene::getAble()
{
	return isAble;
}

void TreeScene::setAble(bool a)
{
	isAble = a;
}

void TreeScene::createLights() {
	new Light(&sdlutils().images().at("luz_amarilla"), 200, 100, 400, 100, mngr_);
	new Light(&sdlutils().images().at("luz_amarilla"), 300, 1080 - 350, 500, 100, mngr_);
	new Light(&sdlutils().images().at("luz_naranja"), 1920 - 100, 1080 - 300, 600, 100, mngr_);
	new Light(&sdlutils().images().at("luz_naranja"), 1920 - 200, 100, 500, 100, mngr_);
													   
}