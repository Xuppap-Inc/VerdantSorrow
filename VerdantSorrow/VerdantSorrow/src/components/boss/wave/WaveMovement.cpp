#include "waveMovement.h"

#include "../../Transform.h"
#include "../../../ecs/Entity.h"

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
	auto pos = tr_->getPos();

	vel = new Vector2D(speed_ * dir_, 0);

	if (pos.getX() < 0 - tr_->getWidth() || pos.getX() > sdlutils().width()) {
		ent_->setAlive(false);
	}
}