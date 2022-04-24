#include "TreeMovement.h"
#include "../../../components/Transform.h"
#include "../../../ecs/Entity.h"

TreeMovement::TreeMovement() :
	tr_(),
	playerTr_(),
	offsetX_(),
	movementDir_(),
	isNextToPlayer_(),
	moveActive_(true),
	movingToCenter_(false)
{
}
TreeMovement::TreeMovement(Transform* playerTransform_, float followVelocity) :
	tr_(),
	movementDir_(),
	isNextToPlayer_(),
	playerTr_(playerTransform_),
	offsetX_(0),
	moveActive_(true),
	followVel_(followVelocity),
	movingToCenter_(false)
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

	offsetX_ = playerWidth / 2 + treeWidth / 4;
}

void TreeMovement::update()
{
	auto playerPos = playerTr_->getPos();
	auto playerVel = playerTr_->getVel();
	auto& treePos = tr_->getPos();
	auto& treeVel = tr_->getVel();

	auto playerWidth = playerTr_->getWidth();
	auto treeWidth = tr_->getWidth();

	if (!movingToCenter_)
		followPlayer(playerPos, playerWidth, treePos, treeWidth, treeVel);
	
	else {
	
		moveToCenter(treePos, treeWidth, treeVel);
	}
}

void TreeMovement::moveToCenter(Vector2D& treePos, float treeWidth, Vector2D& treeVel)
{
	//distancia entre el arbol y el centro
	float distance = sdlutils().width() / 2 - (treePos.getX() + treeWidth / 2);

	//si está a melee, se para
	if (abs(distance) <= 10) {

		movingToCenter_ = false;
	}

	else if (distance > 0) movementDir_ = 1;
	else if (distance < 0) movementDir_ = -1;

	treeVel = Vector2D(movementDir_ * followVel_, 0);
}

void TreeMovement::followPlayer(Vector2D& playerPos, float playerWidth, Vector2D& treePos, float treeWidth, Vector2D& treeVel)
{
	//distancia entre el arbol y el jugador
	float distance = (playerPos.getX() + playerWidth / 2) - (treePos.getX() + treeWidth / 2);

	//si está a melee, se para
	if (abs(distance) <= offsetX_) movementDir_ = 0;

	else if (distance > 0) movementDir_ = 1;
	else if (distance < 0) movementDir_ = -1;

	if (moveActive_) treeVel = Vector2D(movementDir_ * followVel_, 0);
	else treeVel = Vector2D(0, 0);
}

void TreeMovement::moveToCenter()
{
	movingToCenter_ = true;
}

bool TreeMovement::hasFinishedMovingToCenter()
{
	return !movingToCenter_;
}
