#include "FlyMovement.h"
#include "../../Transform.h"
#include "../../../ecs/Entity.h"

FlyMovement::FlyMovement() : tr_(), dir_()
{
}

FlyMovement::FlyMovement(int dir) : tr_(), dir_(dir)
{
}

FlyMovement::~FlyMovement()
{
}

void FlyMovement::initComponent() {
	tr_ = ent_->getComponent<Transform>();
}

void FlyMovement::update()
{
	auto& vel = tr_->getVel();
	vel = new Vector2D(dir_ * 2, 0);
}