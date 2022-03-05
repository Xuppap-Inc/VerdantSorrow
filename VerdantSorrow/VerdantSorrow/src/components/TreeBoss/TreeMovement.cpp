#include "TreeMovement.h"
#include "../../components/Transform.h"
#include "../../ecs/Entity.h"

TreeMovement::TreeMovement() :
	tr_(),
	playerTr_(),
	offsetX_(),
	offsetY_()
{
}
TreeMovement::TreeMovement(Transform* playerTransform_, float offsetX, float offsetY, Vector2D velocity, float followVelocity) :
	tr_(),
	playerTr_(playerTransform_),
	offsetX_(offsetX),
	offsetY_(offsetY),
	vel_(velocity),
	followVel_(followVelocity)
{
}
TreeMovement::~TreeMovement() {}

void TreeMovement::initComponent()
{
	tr_ = ent_->getComponent<Transform>(); //Guarda el transform de la mosca
	assert(tr_ != nullptr);
	assert(playerTr_ != nullptr);
}

void TreeMovement::update()
{

	auto playerPos = playerTr_->getPos();
	auto playerVel = playerTr_->getVel();
	auto& flyPos = tr_->getPos();
	auto& flyVel = tr_->getVel();

	Vector2D distance = (playerPos - flyPos);
	auto velX = distance.normalize().getX();
	if (abs(distance.getX()) <= offsetX_)
	{
		velX = 0.0f;
	}

	flyVel = Vector2D((velX * followVel_), distance.getY() + offsetY_);
}