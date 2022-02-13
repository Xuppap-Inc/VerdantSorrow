#include "PlayerCtrl.h"
#include "../sdlutils/InputHandler.h"
#include "Container.h"


PlayerCtrl::~PlayerCtrl()
{
}

void PlayerCtrl::handleInput(Container* o)
{
	auto& ihdlr = ih();

	if (ihdlr.keyDownEvent()) {
		if (ihdlr.isKeyDown(SDLK_w)) {
			
		}
		if (ihdlr.isKeyDown(SDLK_a)) {
			o->getVel().set(o->getVel() + Vector2D(-speed_, 0));
		}
		else if (ihdlr.isKeyDown(SDLK_d)) {
			o->getVel().set(o->getVel() + Vector2D(speed_, 0));
		}
	}
}
