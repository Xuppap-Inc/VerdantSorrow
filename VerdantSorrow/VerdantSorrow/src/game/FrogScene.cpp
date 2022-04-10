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


#include "CollisionManager.h"
#include "../components/boss/wave/WaveSpawner.h"
#include "SceneManager.h"
#include "../game/CollisionChecker.h"
#include "../components/fondos/ParticleSystem.h"
#include "../components/fondos/Light.h"


void FrogScene::init()
{
	Scene::init();
	//Para gestionar las colisiones
	CollisionManager* colManager = new CollisionManager();

	background();

	waveSpawerGenerator(colManager);

	//Se crea el jugador
	player = mngr_->addEntity();
	playerGenerator(colManager, player);

	frogGenerator(colManager, player);
	auto particles = mngr_->addEntity();
	particles->addComponent<Transform>(Vector2D(0, 0), Vector2D(), sdlutils().width(), sdlutils().height(), 0.0f);
	particles->addComponent<FramedImage>(&sdlutils().images().at("particles"), 14, 5, 2000, 32, "particles");

	colCheck_ = new CollisionChecker(colManager, mngr_);
	ParticleSystem* particlesys = new ParticleSystem(&sdlutils().images().at("particle"), 100, mngr_);
	particlesys->createParticles();

	createLights();
}

void FrogScene::update()
{
	auto health = 0;
	auto bossHealth = 0;
	if(player!=nullptr)
	 health = player->getComponent<PlayerAttributes>()->getLives();
	if(Frog!=nullptr)
	 bossHealth = Frog->getComponent<BossAtributos>()->getLife();
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
		if (health <= 0) sC().changeFrogEssenceState(true);
		if (bossHealth <= 0)sC().changeStatePlayerInBoss(false);
			
		setAble(false);
		sC().decideScene();
	}
}

void FrogScene::waveSpawerGenerator(CollisionManager*& colManager)
{
	//se crea wave spwner
	auto waveSp = mngr_->addEntity();
	waveSp->addComponent<WaveSpawner>(colManager);
	mngr_->setHandler(ecs::_WAVE_GENERATOR, waveSp);
	waveSp->addToGroup(ecs::_BOSS_GRP);
}

void FrogScene::background()
{
	Scene::background("fondo1");
}

void FrogScene::frogGenerator(CollisionManager* colManager, Entity* player_) {

	Frog = mngr_->addEntity();
	Frog->addToGroup(ecs::_BOSS_GRP);
	mngr_->setHandler(ecs::_FROGBOSS, Frog);
	auto FrogAtribs = Frog->addComponent<BossAtributos>(40);

	auto frogH = 400;
	auto frogW = frogH * 1.11f;
	auto FrogX = sdlutils().width() / 2 - 25;
	auto FrogY = sdlutils().height() - frogH;
	//auto FrogTr = Frog->addComponent<Transform>(Vector2D(FrogX, FrogY), Vector2D(), frogW, frogH, 0.0f);
	auto FrogTr = Frog->addComponent<Transform>();

	FrogTr->init(Vector2D(FrogX, FrogY), Vector2D(), frogW, frogH, 0.0f, 400.0/300.0);

	Frog->addComponent<FramedImage>(&sdlutils().images().at("ranajump"), 6, 6, 5000, 32, "ranajump");

	auto windowScaleHeight_ = sdlutils().height() / 1080.0f;
	auto windowScaleWidth_ = sdlutils().width() / 1920.0f;
	//Se añade un collider a la rana
	auto frogCollider = Frog->addComponent<RectangleCollider>(FrogTr->getWidth() - (150 * windowScaleWidth_), 
		FrogTr->getHeight() - (200 * windowScaleHeight_), 0, 75*windowScaleHeight_);
	frogCollider->setIsTrigger(true);
	colManager->addCollider(frogCollider);
		
	Frog->addComponent<SimpleGravity>(1);
	Frog->addComponent<CollideWithBordersBoss>();

	Frog->addComponent<FrogAttackManager>(colManager);

	Frog->addComponent<BossHPBar>();
	auto frogHPBar = mngr_->addEntity();
	frogHPBar->addComponent<BossHPBar>();
	frogHPBar->addToGroup(ecs::_UI_GRP);
}

bool FrogScene::getAble()
{
	return isAble;
}

void FrogScene::setAble(bool a)
{
	isAble = a;
}

void FrogScene::createLights() {
	new Light(&sdlutils().images().at("luz_cyan"), 100, sdlutils().height() - 200, 500, 100, mngr_);
	new Light(&sdlutils().images().at("luz_cyan"),  300, sdlutils().height() - 200, 500, 100, mngr_);
	new Light(&sdlutils().images().at("luz_cyan"), 500, sdlutils().height() - 200, 500, 100, mngr_);
	new Light(&sdlutils().images().at("luz_cyan"),  700, sdlutils().height() - 200, 500, 100, mngr_);

	new Light(&sdlutils().images().at("luz_negro"), -200, sdlutils().height() - 200, 400, 100, mngr_);
	new Light(&sdlutils().images().at("luz_negro"), -200, sdlutils().height() - 400, 400, 100, mngr_);
	new Light(&sdlutils().images().at("luz_negro"), -200, sdlutils().height() - 600, 400, 100, mngr_);
	new Light(&sdlutils().images().at("luz_negro"), -200, sdlutils().height() - 800, 400, 100, mngr_);

	new Light(&sdlutils().images().at("luz_negro"), sdlutils().width()-200, sdlutils().height() - 200, 400, 100, mngr_);
	new Light(&sdlutils().images().at("luz_negro"), sdlutils().width()-200, sdlutils().height() - 400, 400, 100, mngr_);
	new Light(&sdlutils().images().at("luz_negro"), sdlutils().width()-200, sdlutils().height() - 600, 400, 100, mngr_);
	new Light(&sdlutils().images().at("luz_negro"), sdlutils().width()-200, sdlutils().height() - 800, 400, 100, mngr_);

	new Light(&sdlutils().images().at("luz_verde"), 1100, 300, 300, 100, mngr_);
	new Light(&sdlutils().images().at("luz_verde"), 50, 200, 350, 100, mngr_);
	new Light(&sdlutils().images().at("luz_verde"), 400, 100, 200, 100, mngr_);
	new Light(&sdlutils().images().at("luz_verde"), 850, 50, 400, 100, mngr_);
	new Light(&sdlutils().images().at("luz_verde"), 600, 150, 350, 100, mngr_);
}