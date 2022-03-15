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
#include "../BossAtributos.h"
#include <time.h>
#include <random>




LanternSpawner::LanternSpawner(CollisionManager* colManager)
	: colManager_(colManager), framedImg_(), lanternWidht_(50)
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

void LanternSpawner::createLantern(int x,int y)
{
	//distribucion random de intervalo variable en distintas llamadas
	std::random_device seed;
	std::mt19937 gen{ seed() };
	std::uniform_int_distribution <>myrand(0, sdlutils().width());

	//crea entidad linterna
	auto Lantern = mngr_->addEntity();

	//atributos de linterna
	auto LanternAtribs = Lantern->addComponent<BossAtributos>();
	auto LanternTr = Lantern->addComponent<Transform>();
	auto LanternX = x;
	auto LanternY = y;
	std::cout << LanternX << std::endl;


	//damos paramtros iniciales
	LanternTr->init(Vector2D(LanternX, LanternY), Vector2D(), lanternWidht_, 50, 0.0f);
	//de momento con un color pocho
	Lantern->addComponent<RectangleRenderer>(SDL_Color());

	//collider de la lampara
	auto LanternCollider = Lantern->addComponent <RectangleCollider>
		(LanternTr->getWidth(), LanternTr->getHeight());
	//se hace trigger
	LanternCollider->setIsTrigger(true);
	//le pasamos el collider al manager
	colManager_->addCollider(LanternCollider);

	
}
