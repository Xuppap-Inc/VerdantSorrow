#include "FlyMovement.h"
#include "../../Transform.h"
#include "../../../ecs/Entity.h"
#include "../../../ecs/Manager.h"

FlyMovement::FlyMovement() : tr_(), dir_(1), playerTr_()
{
}

FlyMovement::~FlyMovement()
{
}

void FlyMovement::initComponent() {
	
	tr_ = ent_->getComponent<Transform>();
	playerTr_ = mngr_->getHandler(ecs::_PLAYER)->getComponent<Transform>();

	bool correct = tr_ != nullptr && playerTr_ != nullptr;
}

void FlyMovement::update()
{
	auto& vel = tr_->getVel();

	dir_ = (playerTr_->getPos().getX() - tr_->getPos().getX()) / abs(playerTr_->getPos().getX() - tr_->getPos().getX());

	vel = new Vector2D(dir_ * SPEED, 0);
}