#include "LanternSpawner.h"


#include "../../../sdlutils/SDLUtils.h"
#include "../../../ecs/Entity.h"
#include "../../../ecs/Manager.h"
#include "../../Transform.h"
#include "../../RectangleCollider.h"
#include "../../RectangleRenderer.h"
#include "../../../game/CollisionManager.h"
#include "../BossAtributos.h"


LanternSpawner::LanternSpawner(CollisionManager* collManager)
{
}

LanternSpawner::~LanternSpawner()
{
}

void LanternSpawner::initComponent()
{
}

void LanternSpawner::createLantern(int x)
{

	//crea entidad linterna
	auto lantern = mngr_->addEntity();

	//atributos del boss
	auto lanternAtribs = lantern->addComponent<BossAtributos>();
	auto lanternTr = lantern->addComponent<Transform>();
	auto lanternX = x;
	auto lanternY = sdlutils().height();

	//damos paramtros iniciales
	lanternTr->init(Vector2D(lanternX, lanternY), Vector2D(), lanternWidht_, 500, 0.0f);
	//de momento con un color pocho
	lantern->addComponent<RectangleRenderer>(SDL_Color());

	//collider de la lampara
	auto lanternCollider = lantern->addComponent <RectangleCollider>
		(lanternTr->getWidth(), lanternTr->getHeight());
	//se hace trigger
	lanternCollider->setIsTrigger(true);
	//le pasamos el collider al manager
	collManager_->addCollider(lanternCollider);

	
}
