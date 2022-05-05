#include "Spikes.h"
#include "../../../ecs/Entity.h"
#include "../../../ecs/Manager.h"
#include "../../Transform.h"
#include "../wave/WaveMovement.h"
#include "../frog_boss/TongueAttack.h"
#include "../finalBoss/ClapAttack.h"
#include "../../SimpleGravity.h"
#include "../../player/PlayerAttributes.h"


Spikes::Spikes(CollisionManager* colManager,bool movement) : tr_(nullptr), colMan_(colManager), collider_(nullptr), activationDistance_(100), moving_(false), spikeVelocity_(2.5),movement_(movement)
{
}

Spikes::~Spikes()
{
}

void Spikes::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	collider_ = ent_->getComponent<RectangleCollider>();
	gr_ = ent_->getComponent<SimpleGravity>();

	assert(tr_ != nullptr && collider_ != nullptr);
	collider_->setIsTrigger(true);
	if(gr_)
	gr_->setActive(false);
}

void Spikes::update()
{
	if (movement_) {
		auto playerTr = mngr_->getHandler(ecs::_PLAYER)->getComponent<Transform>();
		if (!moving_ && tr_->getPos().getX() - playerTr->getPos().getX() + playerTr->getWidth() <= activationDistance_) {
			moving_ = true;
			gr_->setActive(true);
			tr_->getVel().setY(spikeVelocity_);
		}
	}
	
	
}


