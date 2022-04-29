#include "LanternSpawner.h"


#include "../../../sdlutils/SDLUtils.h"
#include "../../../ecs/Entity.h"
#include "../../../ecs/Manager.h"
#include "../../Transform.h"
#include "../../RectangleCollider.h"
#include "../../RectangleRenderer.h"
#include "../../../game/CollisionManager.h"
#include "TreeMovement.h"
#include "TreeAttackManager.h"
#include "LanternMovement.h"
#include "../BossAtributos.h"
#include <time.h>
#include <random>




LanternSpawner::LanternSpawner(CollisionManager* colManager)
	: colManager_(colManager), framedImg_(), lanternWidth_(100)
{
}

LanternSpawner::~LanternSpawner()
{
	
}

void LanternSpawner::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
}

void LanternSpawner::update() {
	
	
}

void LanternSpawner::createLantern(int x,int y,bool quieta)
{
	//distribucion random de intervalo variable en distintas llamadas
	std::random_device seed;
	std::mt19937 gen{ seed() };
	std::uniform_int_distribution <>myrand(0, sdlutils().width());

	//crea entidad linterna
	auto lantern = mngr_->addEntity();

	mngr_->setHandler(ecs::_LANTERN, lantern);

	//atributos de linterna
	auto LanternAtribs = lantern->addComponent<BossAtributos>(1);
	auto LanternTr = lantern->addComponent<Transform>();
	auto LanternX = x;
	auto LanternY = y;


	//damos paramtros iniciales
	LanternTr->init(Vector2D(LanternX, LanternY), Vector2D(), lanternWidth_, lanternWidth_, 0.0f);
	//de momento con un color pocho
	lantern->addComponent<RectangleRenderer>(SDL_Color());

	//collider de la lampara
	auto LanternCollider = lantern->addComponent <RectangleCollider>
		(LanternTr->getWidth(), LanternTr->getHeight());
	//se hace trigger
	LanternCollider->setIsTrigger(true);
	//le pasamos el collider al manager
	colManager_->addCollider(LanternCollider);
	//la damos movimiento
	lantern->addComponent<LanternMovement>();

	
}
