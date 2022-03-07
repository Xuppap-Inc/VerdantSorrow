#include "waveMovement.h"

#include "../../../Transform.h"
#include "../../../../ecs/Entity.h"

WaveMovement::WaveMovement(int dir, float speed) : tr_(), dir_(dir), speed_(speed), lastTime_(0)
{
}
WaveMovement::~WaveMovement()
{
}

void WaveMovement::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
	lastTime_ = sdlutils().currRealTime();
}


void WaveMovement::update()
{
	auto& vel = tr_->getVel();

	vel = new Vector2D(speed_ * dir_, 0);

	if (sdlutils().currRealTime() - lastTime_ > 3000) {
		ent_->setAlive(false);
	}
}