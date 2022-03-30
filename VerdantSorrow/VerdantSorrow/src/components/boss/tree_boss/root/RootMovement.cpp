#include "RootMovement.h"

#include "../../../Transform.h"
#include "../../../../ecs/Entity.h"


RootMovement::RootMovement() : tr_(), col_(), speed_(0.25), lastTime_(0)
{
}
RootMovement::~RootMovement()
{
}

void RootMovement::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
	col_ = ent_->getComponent<RectangleCollider>();
	col_->setActive(false);
}

void RootMovement::update()
{
	if (speed_ == 0) {
		col_->setActive(false);	
		if (sdlutils().currRealTime() - lastTime_ > 700) {
			ent_->setAlive(false);
		}
	}
	else {
		if (tr_->getPos().getY() < 100) {
			speed_ = 0;
			lastTime_ = sdlutils().currRealTime();
		}
		else if (tr_->getPos().getY() < sdlutils().height() - 30) {
			speed_ = 50;
			col_->setActive(true);
		}
		auto& vel = tr_->getVel();
		vel = new Vector2D(0, -speed_);
	}
}