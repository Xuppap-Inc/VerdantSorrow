#include "RootAutoAim.h"

#include "../../../sdlutils/SDLUtils.h"
#include "../../../ecs/Entity.h"
#include "../../../ecs/Manager.h"
#include "../../Transform.h"
#include "../../RectangleCollider.h"
#include "../../RectangleRenderer.h"
#include "../../../game/CollisionManager.h"
#include "../BossAtributos.h"

RootAutoAim::RootAutoAim(ecs::Entity* player) : tr_(), lastTime_(0), rootSpawner_(), attacking_(false), rootPos_(-1), rootW_(0), TIME_BETWEEN_ROOTS(1000), player_(player), playerTr_()
{
}

RootAutoAim::~RootAutoAim()
{
}

void RootAutoAim::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	rootSpawner_ = ent_->getComponent<RootSpawner>();
	playerTr_ = player_->getComponent<Transform>();

	bool comps = tr_ != nullptr && rootSpawner_ != nullptr && playerTr_ != nullptr;
	assert(comps);
}

void RootAutoAim::update()
{
	if (attacking_ && sdlutils().currRealTime() - lastTime_ > TIME_BETWEEN_ROOTS) {
	
		rootPos_ = playerTr_->getPos().getX() + playerTr_->getWidth() / 2 - rootW_ / 2;
		rootSpawner_->createRoot(rootPos_);

		lastTime_ = sdlutils().currRealTime();
	}
}

void RootAutoAim::cancelAttack()
{
	attacking_ = false;
}

void RootAutoAim::attack()
{
	attacking_ = true;
	rootW_ = rootSpawner_->getRootWidth();
}
