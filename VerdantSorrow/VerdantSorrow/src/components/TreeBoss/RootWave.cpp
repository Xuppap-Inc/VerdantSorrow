#include "RootWave.h"

#include "../../sdlutils/SDLUtils.h"
#include "../../ecs/Entity.h"
#include "../../ecs/Manager.h"
#include "../Transform.h"
#include "../RectangleCollider.h"
#include "../RectangleRenderer.h"
#include "../../game/CollisionManager.h"
#include "../FrogBoss/BossAtributos.h"

RootWave::RootWave(CollisionManager* collManager) : collManager_(collManager), tr_(), lastTime(0)
{
}

RootWave::~RootWave()
{
}

void RootWave::initComponent()
{
	tr_ = ent_->getComponent<Transform>();

	bool comps = tr_ != nullptr;
	assert(comps);
}

void RootWave::update()
{
}

void RootWave::attack(int dir)
{
	auto rootPos = tr_->getPos().getX();
	lastTime = sdlutils().currRealTime();
	createRoot(rootPos);
	rootPos += (25 + 5) * dir;

	while (rootPos > 0 && rootPos < sdlutils().width()) {
	
		createRoot(rootPos);
		rootPos += (25 + 5) * dir;

		lastTime = sdlutils().currRealTime();
	}
}

void RootWave::createRoot(int x)
{
	//Se crea la raiz
	auto Root = mngr_->addEntity();
	//Se añaden los atributos del boss que están junto al transform
	auto RootAtribs = Root->addComponent<BossAtributos>();
	auto RootTr = Root->addComponent<Transform>();
	auto RootX = x;
	auto RootY = sdlutils().height() - 50;
	//Se le dan las posiciones iniciales, velocidad, ancho y alto a la raiz
	int rootWidth_ = 25;
	RootTr->init(Vector2D(RootX, RootY), Vector2D(), rootWidth_, 500, 0.0f);
	//Se le añade un color inicial a la raiz
	Root->addComponent<RectangleRenderer>(SDL_Color());

	//Se añade un collider a la onda
	auto RootCollider = Root->addComponent<RectangleCollider>(RootTr->getWidth(), RootTr->getHeight());
	RootCollider->setIsTrigger(true);
	//Se añade el collider al colliderGameManager
	collManager_->addCollider(RootCollider);
	//Se añade el movimiento horizontal
	Root->addComponent<RootMovement>();
}
