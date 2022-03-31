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

void ClapAttack::goDiagonal()
{
	collider_->setActive(false);
	int xObjective;
	if (!leftHand_) xObjective = 0;
	else xObjective = sdlutils().width() - tr_->getWidth();
	if (abs(tr_->getPos().getX() - xObjective) > 5 || abs(tr_->getPos().getY() - sdlutils().height() + tr_->getHeight()) > 5) {
		Vector2D dir = Vector2D(xObjective - tr_->getPos().getX(), sdlutils().height() - tr_->getPos().getY() - tr_->getHeight());/*initialPos - tr_->getPos();*/
		tr_->getVel().set(dir.normalize() * handSpeed);
	}
	else {
		tr_->getVel().set(Vector2D(0, 0));
		tr_->getPos().set(Vector2D(xObjective, sdlutils().height() - tr_->getHeight()));
		changeState(CENTER);
	}
}

void ClapAttack::goCenter(bool quemado)
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
			changeState(REPOSOSUELO);
			SoundEffect* s = &sdlutils().soundEffects().at("sfx_manos_attack2");
			s->play();
			if (quemado) {
				SoundEffect* s2 = &sdlutils().soundEffects().at("sfx_manos_fire_clap");
				s2->play();
			}
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

void ClapAttack::stayFloor() {
	collider_->setIsTrigger(false);
	if(sdlutils().currRealTime() > lastTimeFloor + cooldoownInFloor)
		changeState(BACK);
}
