#include "ClapAttack.h"
#include "../../../ecs/Entity.h"
#include "../../Transform.h"
#include "../../RectangleCollider.h"
#include "../../../sdlutils/SDLUtils.h"

ClapAttack::ClapAttack(bool leftHand) : leftHand_(leftHand), tr_(nullptr), state_(REPOSO), initialPos()
{
}

ClapAttack::~ClapAttack()
{
}

void ClapAttack::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	collider_ = ent_->getComponent<RectangleCollider>();
	assert(tr_ != nullptr, collider_ != nullptr);

	initialPos = Vector2D(tr_->getPos().getX(), tr_->getPos().getY());
}

void ClapAttack::goSide()
{
	int objectivePos;

	collider_->setActive(false);

	if (leftHand_) {
		objectivePos = sdlutils().width() - tr_->getWidth();

		if (tr_->getPos().getX() < objectivePos) {
			tr_->getVel().set(Vector2D(handSpeed, 0));
		}
		else {
			tr_->getVel().set(Vector2D(0, 0));
			tr_->getPos().setX(objectivePos);
			changeState(DOWN);
		}
	}
	else {
		objectivePos = 0;

		if (tr_->getPos().getX() > objectivePos) {
			tr_->getVel().set(Vector2D(-handSpeed, 0));
		}
		else {
			tr_->getVel().set(Vector2D(0, 0));
			tr_->getPos().setX(objectivePos);
			changeState(DOWN);
		}
	}
}

void ClapAttack::goDown()
{
	int objectivePos = sdlutils().height() - tr_->getWidth();


	if (tr_->getPos().getY() < objectivePos) {
		tr_->getVel().set(Vector2D(0, handSpeed));
	}
	else {
		tr_->getVel().set(Vector2D(0, 0));
		tr_->getPos().setY(objectivePos);
		changeState(CENTER);
	}
}

void ClapAttack::goCenter()
{
	int objectivePos;

	collider_->setActive(true);
	collider_->setIsTrigger(true);


	if (leftHand_) {
		objectivePos = sdlutils().width() / 2;

		if (tr_->getPos().getX() > objectivePos) {
			tr_->getVel().set(Vector2D(-handSpeed * 2, 0));
		}
		else {
			tr_->getVel().set(Vector2D(0, 0));
			tr_->getPos().setX(objectivePos);
			changeState(BACK);
		}
	}
	else {
		objectivePos = sdlutils().width() / 2 - tr_->getWidth();

		if (tr_->getPos().getX() < objectivePos) {
			tr_->getVel().set(Vector2D(handSpeed * 2, 0));
		}
		else {
			tr_->getVel().set(Vector2D(0, 0));
			tr_->getPos().setX(objectivePos);
			changeState(BACK);
		}
	}
}

void ClapAttack::goBack()
{
	if (abs(tr_->getPos().getX() - initialPos.getX()) > 5 || abs(tr_->getPos().getY() - initialPos.getY()) > 5) {
		Vector2D dir = initialPos - tr_->getPos();
		tr_->getVel().set(dir.normalize() * handSpeed);
	}
	else {
		tr_->getVel().set(Vector2D(0, 0));
		tr_->getPos().set(initialPos);
		changeState(FIN);
	}
}
