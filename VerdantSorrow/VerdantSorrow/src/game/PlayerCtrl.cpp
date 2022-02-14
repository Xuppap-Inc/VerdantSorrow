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
		else if (ihdlr.isKeyDown(SDLK_a)) {
			
		}
		else if (ihdlr.isKeyDown(SDLK_d)) {
			
		}
	}
}
