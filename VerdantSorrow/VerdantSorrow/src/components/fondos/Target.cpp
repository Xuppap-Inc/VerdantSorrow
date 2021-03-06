#include "Target.h"

#include "../../ecs/Manager.h"
#include "../../ecs/Entity.h"
#include "../../sdlutils/SDLUtils.h"	
#include "../Transform.h"
#include "../Image.h"


Target::Target(int time, Transform* target) : tr_(), time_(time), img_(), target_(target)
{
	
}
Target::~Target() {
}

void Target::initComponent() {
	tr_ = ent_->getComponent<Transform>();
	img_ = ent_->getComponent<Image>();
	timer_ = mngr_->addTimer();
}

void Target::update() {
	if (timer_->currTime() >= time_) {
		bool Xbuena = false;
		bool Ybuena = false;
		if ((target_->getPos().getX() + target_->getWidth() / 2) - tr_->getPos().getX() > 20)
			tr_->getVel().setX(10 + abs((target_->getPos().getX() + target_->getWidth() / 2) - tr_->getPos().getX()) * 0.05);
		else if ((target_->getPos().getX() + target_->getWidth() / 2) - tr_->getPos().getX() < -20)
			tr_->getVel().setX(-10 + abs((target_->getPos().getX() + target_->getWidth() / 2) - tr_->getPos().getX()) * -0.05);
		else
			Xbuena = true;
		if ((target_->getPos().getY() + target_->getHeight() / 2) - tr_->getPos().getY() > 20)
			tr_->getVel().setY(20 + abs((target_->getPos().getY() + target_->getHeight() / 2) - tr_->getPos().getY()) * 0.05);
		else if ((target_->getPos().getY() + target_->getHeight() / 2) - tr_->getPos().getY() < -20)
			tr_->getVel().setY(-10 + abs((target_->getPos().getY() + target_->getHeight() / 2) - tr_->getPos().getY()) * -0.05);
		else
			Ybuena = true;
		if (Xbuena && Ybuena)
			ent_->setAlive(false);
	}
}
