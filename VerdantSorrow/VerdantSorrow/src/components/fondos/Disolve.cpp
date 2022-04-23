#include "Disolve.h"

#include "../../ecs/Entity.h"
#include "../../ecs/Manager.h"
#include "../../sdlutils/SDLUtils.h"	
#include "../Transform.h"
#include "../Image.h"


Disolve::Disolve(int startFadingTime) : tr_(), startFadingTime_(startFadingTime), img_(), fading_(false)
{
	
}
Disolve::~Disolve() {
}

void Disolve::initComponent() {
	img_ = ent_->getComponent<Image>();
	timer_ = mngr_->addTimer();
}

void Disolve::update() {
	
	if (timer_->currTime() >= startFadingTime_) {
		
		if (!fading_) {
			img_->fadeOut();
			fading_ = true;
		}

		//no se si esta linea es lo mismo que el alpha, asi que te lo dejo aqui
		if (timer_->currTime() - startFadingTime_ > 100) {
			ent_->setAlive(false);
		}
	}
}
