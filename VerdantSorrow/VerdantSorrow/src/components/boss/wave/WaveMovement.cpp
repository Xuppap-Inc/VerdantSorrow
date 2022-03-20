#include "waveMovement.h"

#include "../../Transform.h"
#include "../../../ecs/Entity.h"

WaveMovement::WaveMovement(Vector2D dir, float speed) : tr_(), dir_(dir), speed_(speed), lastTime_(0)
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

	if (dir_.magnitude() != 0)
		vel.set(dir_.normalize() * speed_);

	if (pos.getX() < 0 - tr_->getWidth() || pos.getX() > sdlutils().width()
		|| pos.getY() < -tr_->getHeight() || pos.getY() > sdlutils().height()) {
		ent_->setAlive(false);
	}
}