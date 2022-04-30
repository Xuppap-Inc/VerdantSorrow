#include "ShowAtOppositeSide.h"
#include <cassert>

#include "../../ecs/Entity.h"
#include "../../sdlutils/SDLUtils.h"
#include "../Transform.h"
#include "../../ecs/Manager.h"

ShowAtOpposideSide::ShowAtOpposideSide(bool camera) : tr_()
{
}
ShowAtOpposideSide::~ShowAtOpposideSide() {
}

void ShowAtOpposideSide::initComponent() {
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
	auto camera = mngr_->getHandler(ecs::_hdlr_CAMERA);
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
