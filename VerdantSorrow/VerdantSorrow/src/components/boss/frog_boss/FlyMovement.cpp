#include "FlyMovement.h"
#include "../../Transform.h"
#include "../../../ecs/Entity.h"
#include "../../../ecs/Manager.h"

FlyMovement::FlyMovement() : tr_(), dirX_(1), dirY_(1), playerTr_(), spaceWhereExplotes(10)
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

	//Calcula velocidad y le da velocidad a la mosca
	dirX_ = (playerTr_->getPos().getX() - tr_->getPos().getX()) / abs(playerTr_->getPos().getX() - tr_->getPos().getX());
	dirY_ = (playerTr_->getPos().getY() - tr_->getPos().getY()) / abs(playerTr_->getPos().getY() - tr_->getPos().getY());
	vel = new Vector2D(dirX_ * SPEED, dirY_ * SPEED);
	
	//Si queda poco espacio explota y muere
	if (abs(playerTr_->getPos().getX() - tr_->getPos().getX()) < spaceWhereExplotes || abs(playerTr_->getPos().getY() - tr_->getPos().getY() < spaceWhereExplotes))
		ent_->setAlive(false);


	
}