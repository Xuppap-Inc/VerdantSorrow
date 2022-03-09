#include "TreeMovement.h"
#include "../../../components/Transform.h"
#include "../../../ecs/Entity.h"

TreeMovement::TreeMovement() :
	tr_(),
	playerTr_(),
	offsetX_(),
	movementDir_(),
	isNextToPlayer_(),
	moveActive_(true)
{
}
TreeMovement::TreeMovement(Transform* playerTransform_, float followVelocity) :
	tr_(),
	movementDir_(),
	isNextToPlayer_(),
	playerTr_(playerTransform_),
	offsetX_(0),
	moveActive_(true),
	followVel_(followVelocity)
{
}
TreeMovement::~TreeMovement() {}

void TreeMovement::initComponent()
{
	tr_ = ent_->getComponent<Transform>(); //Guarda el transform de la mosca
	assert(tr_ != nullptr);
	assert(playerTr_ != nullptr);

	auto playerWidth = playerTr_->getWidth();
	auto treeWidth = tr_->getWidth();

	offsetX_ = playerWidth / 2 + treeWidth / 2 + 10;
}

void TreeMovement::update()
{
	auto playerPos = playerTr_->getPos();
	auto playerVel = playerTr_->getVel();
	auto& treePos = tr_->getPos();
	auto& treeVel = tr_->getVel();

	auto playerWidth = playerTr_->getWidth();
	auto treeWidth = tr_->getWidth();

	//distancia entre el arbol y el jugador
	float distance = (playerPos.getX() + playerWidth/2) - (treePos.getX() + treeWidth / 2);

	//si está a melee, se para
	if (abs(distance) <= offsetX_) movementDir_ = 0;

	else if (distance > 0) movementDir_ = 1;
	else if (distance < 0) movementDir_ = -1;

	if (moveActive_) treeVel = Vector2D(movementDir_ * followVel_, 0);
	else treeVel = Vector2D(0, 0);
}