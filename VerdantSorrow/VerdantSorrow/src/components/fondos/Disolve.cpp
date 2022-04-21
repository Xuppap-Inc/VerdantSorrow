#include "Disolve.h"

#include "../../ecs/Entity.h"
#include "../../ecs/Manager.h"
#include "../../sdlutils/SDLUtils.h"	
#include "../Transform.h"
#include "../Image.h"


Disolve::Disolve(int startFadingTime) : tr_(), startFadingTime_(startFadingTime), img_()
{
	timer_ = new VirtualTimer();
	mngr_->addTimer(timer_);
}
Disolve::~Disolve() {
}

void Disolve::initComponent() {
	img_ = ent_->getComponent<Image>();
	
	timer_->reset();
}

void Disolve::update() {
	
	if (timer_->currTime() >= startFadingTime_) {
		int alpha = 100 - (timer_->currTime() - startFadingTime_);
		if (alpha < 0) {
			ent_->setAlive(false);
		}
	}
}
