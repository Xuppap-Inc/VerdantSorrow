#include "TutorialSpawnRoot.h"

#include "../../sdlutils/SDLUtils.h"
#include "../../ecs/Entity.h"
#include "../../ecs/Manager.h"
#include "../Transform.h"
#include "../RectangleCollider.h"
#include "../../game/CollisionManager.h"
#include "../Image.h"
#include "../boss/tree_boss/root/RootMovement.h"
#include "../boss/bossAtributos.h"


TutorialSpawnRoot::TutorialSpawnRoot(CollisionManager* colManager) : colManager_(colManager), framedImg_(), 
								rootWidth_(60), rootPos_(900),lastTime_()
{
}

TutorialSpawnRoot::~TutorialSpawnRoot()
{
}

void TutorialSpawnRoot::initComponent()
{
	lastTime_ = new VirtualTimer();
	mngr_->addTimer(lastTime_);
}


void TutorialSpawnRoot::update() {

	if (lastTime_->currTime() > 200) {

		if (rootPos_ <= 900) createRoot(rootPos_);
		rootPos_ -= 50;
		if (rootPos_ <= 500) rootPos_ = 1100;
		lastTime_->reset();
	}
}

void TutorialSpawnRoot::createRoot(int x)
{
	//Se crea la raiz
	auto Root = mngr_->addEntity();
	//Se añaden los atributos del boss que están junto al transform
	Root->addComponent<BossAtributos>();
	auto RootTr = Root->addComponent<Transform>();
	auto RootX = x;
	auto RootY = sdlutils().height();
	//Se le dan las posiciones iniciales, velocidad, ancho y alto a la raiz
	RootTr->init(Vector2D(RootX, RootY), Vector2D(), 50, 800, 0.0f);
	//Se le añade un color inicial a la raiz
	Root->addComponent<Image>(&sdlutils().images().at("root"));

	//Se añade un collider a la raiz
	auto RootCollider = Root->addComponent<RectangleCollider>(RootTr->getWidth(), RootTr->getHeight());
	RootCollider->setIsTrigger(true);
	//Se añade el collider al colliderGameManager
	colManager_->addCollider(RootCollider);
	//Se añade el movimiento vertical
	Root->addComponent<RootMovement>();
	Root->addToGroup(ecs::_BOSSELEMENTS_GRP);
}