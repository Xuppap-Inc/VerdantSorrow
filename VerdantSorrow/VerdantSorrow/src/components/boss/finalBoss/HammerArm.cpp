#include "HammerArm.h"
#include "../../../ecs/Entity.h"
#include "../../Transform.h"
#include "../../RectangleCollider.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../../ecs/Manager.h"

HammerArm::HammerArm(bool leftHand) : leftHand_(leftHand), tr_(nullptr), state_(REPOSO), initialPos()
{
}

HammerArm::~HammerArm()
{
}

void HammerArm::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	collider_ = ent_->getComponent<RectangleCollider>();
	playertr_ = mngr_->getHandler(ecs::_PLAYER)->getComponent<Transform>();
	assert(tr_ != nullptr, collider_ != nullptr, playertr_ != nullptr);

	initialPos = Vector2D(tr_->getPos().getX(), tr_->getPos().getY());
}

void HammerArm::goDiagonal()
{
	collider_->setActive(false);

	if (abs(tr_->getPos().getX() - playerXPos) > 5 || abs(tr_->getPos().getY() - sdlutils().height() + tr_->getHeight()) > 5) {
		Vector2D dir = Vector2D(playerXPos - tr_->getPos().getX(), sdlutils().height() - tr_->getPos().getY() - tr_->getHeight());/*initialPos - tr_->getPos();*/
		tr_->getVel().set(dir.normalize() * handSpeed);
	}
	else {
		tr_->getVel().set(Vector2D(0, 0));
		tr_->getPos().set(Vector2D(playerXPos, sdlutils().height() - tr_->getHeight()));
		changeState(HIT);
	}
}


void HammerArm::attack()
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
			lastTimeFloor = sdlutils().currRealTime();
			changeState(REPOSOSUELO);
		}
	}
}

void HammerArm::goBack()
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

void HammerArm::stayFloor() {
	collider_->setIsTrigger(false);
	if (sdlutils().currRealTime() > lastTimeFloor + cooldoownInFloor)
		changeState(BACK);
}
