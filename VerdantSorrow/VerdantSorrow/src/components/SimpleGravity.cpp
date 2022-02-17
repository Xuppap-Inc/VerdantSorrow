#include "SimpleGravity.h"

#include "../ecs/Entity.h"
#include "Transform.h"

SimpleGravity::SimpleGravity(float gravity) : tr_(nullptr), gravity_(gravity), active_(true)
{
}

SimpleGravity::~SimpleGravity()
{
}

void SimpleGravity::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
}

void SimpleGravity::update()
{
	if (active_) tr_->getVel().set(Vector2D(tr_->getVel().getX(), tr_->getVel().getY() + gravity_));
}

void SimpleGravity::setActive(bool set)
{
	active_ = set;
}

bool SimpleGravity::isActive()
{
	return active_;
}


