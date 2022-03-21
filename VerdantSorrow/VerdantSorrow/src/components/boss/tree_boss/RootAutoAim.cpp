#include "RootAutoAim.h"

#include "../../../sdlutils/SDLUtils.h"
#include "../../../ecs/Entity.h"
#include "../../../ecs/Manager.h"
#include "../../Transform.h"
#include "../../RectangleCollider.h"
#include "../../RectangleRenderer.h"
#include "../../../game/CollisionManager.h"
#include "../BossAtributos.h"
#include "TreeMovement.h"

RootAutoAim::RootAutoAim(ecs::Entity* player) : tr_(), lastTime_(0), rootSpawner_(), attacking_(false), rootPos_(-1), rootW_(0), player_(player), playerTr_(), iniTime_(0), movement_(), infiniteDuration_(false),
												secondPhase_(false), lanternTr_()
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

	movement_ = ent_->getComponent<TreeMovement>();

	lanternTr_ = mngr_->getHandler(ecs::_LANTERN)->getComponent<Transform>();

	bool comps = tr_ != nullptr && rootSpawner_ != nullptr && playerTr_ != nullptr && movement_	!= nullptr && lanternTr_ != nullptr;
	assert(comps);
}

void RootAutoAim::update()
{
	if (attacking_ && sdlutils().currRealTime() - lastTime_ > TIME_BETWEEN_ROOTS) {
	
		rootPos_ = playerTr_->getPos().getX() + playerTr_->getWidth() / 2 - rootW_ / 2;

		auto lanternPos_ = lanternTr_->getPos();

		if (!secondPhase_ || (rootPos_ < lanternPos_.getX() - LANTER_SECURE_SPACE || rootPos_ > lanternPos_.getX() + lanternTr_->getWidth() + LANTER_SECURE_SPACE) ) {
			
			rootSpawner_->createRoot(rootPos_);

			lastTime_ = sdlutils().currRealTime();
		}
	}

	if (!infiniteDuration_ && attacking_ && sdlutils().currRealTime() - iniTime_ > DURATION) cancelAttack();
}

void RootAutoAim::cancelAttack()
{
	attacking_ = false;
}

void RootAutoAim::changeToSecondPhase()
{
	secondPhase_ = true;
}

void RootAutoAim::attack(bool infinite)
{
	attacking_ = true;
	infiniteDuration_ = infinite;

	iniTime_ = sdlutils().currRealTime();
	rootW_ = rootSpawner_->getRootWidth();

	movement_->setMoveActive(false);
}
