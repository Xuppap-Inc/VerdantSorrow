#include "waveMovement.h"

#include "../Transform.h"
#include "../../ecs/Entity.h"

WaveMovement::WaveMovement() : tr_(), dir_(), speed_()
{
}

WaveMovement::WaveMovement(int dir, float speed) : tr_(), dir_(dir), speed_(speed)
{
}

void WaveMovement::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
}


void WaveMovement::update()
{
	auto& vel = tr_->getVel();

	vel = new Vector2D(speed_ * dir_, 0);
}
