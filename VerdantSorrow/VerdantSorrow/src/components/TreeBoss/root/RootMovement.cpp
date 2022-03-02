#include "RootMovement.h"

#include "../../Transform.h"
#include "../../../ecs/Entity.h"

RootMovement::RootMovement() : tr_()
{
}

void RootMovement::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
}

void RootMovement::update()
{
	auto& vel = tr_->getVel();

	vel = new Vector2D(0, -1);
}