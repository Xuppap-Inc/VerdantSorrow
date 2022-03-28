#include "ShowAtOppositeSide.h"
#include <cassert>

#include "../../ecs/Entity.h"
#include "../../sdlutils/SDLUtils.h"
#include "../Transform.h"


ShowAtOpposideSide::ShowAtOpposideSide() : tr_()
{
}
ShowAtOpposideSide::~ShowAtOpposideSide() {
}

void ShowAtOpposideSide::initComponent() {
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
}

void ShowAtOpposideSide::update() {
	auto& pos = tr_->getPos();
	auto& vel = tr_->getVel();

	if (pos.getX() < -tr_->getWidth()) {
		pos.set(sdlutils().width(), pos.getY());
	}
	else if (pos.getY() <= -tr_->getHeight()) {
		pos.set(pos.getX(), sdlutils().height());
	}
	else if (pos.getX() > sdlutils().width()) {
		pos.set(-tr_->getWidth(), pos.getY());
	}
	else if (pos.getY() >= sdlutils().height()) {
		pos.set(pos.getX(), -tr_->getHeight());
	}


}
