#include "RootMovement.h"

#include "../../Transform.h"
#include "../../../ecs/Entity.h"

RootMovement::RootMovement() : tr_(), speed(0.25)
{
}
RootMovement::~RootMovement()
{
}

void RootMovement::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
}

void RootMovement::update()
{
	if (speed == 0) {
		//Desactivar el collider
	}
	else {
		if (tr_->getPos().getY() < 100) speed = 0;
		else if (tr_->getPos().getY() < sdlutils().height() - 30)
			speed = 30;
		auto& vel = tr_->getVel();
		vel = new Vector2D(0, -speed);
	}
}