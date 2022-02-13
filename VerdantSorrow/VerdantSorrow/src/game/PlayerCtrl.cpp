#include "PlayerCtrl.h"
#include "../sdlutils/InputHandler.h"
#include "Container.h"


PlayerCtrl::~PlayerCtrl()
{
}

void PlayerCtrl::handleInput(Container* o)
{
	auto& ihdlr = ih();
	b2Vec2 pos1 = { 100,0 };
	b2Vec2 salt = { 0,-50 };
	b2Vec2 vacio= { 0,0 };
	if (ihdlr.keyDownEvent()) {
		if (ihdlr.isKeyDown(SDLK_w)) {
			body_->ApplyLinearImpulse(salt, salt, true);
		}
		if (ihdlr.isKeyDown(SDLK_a)) {
			body_->ApplyLinearImpulse(-pos1, vacio, true);
			//o->getVel().set(o->getVel() + Vector2D(-speed_, 0));
		}
		else if (ihdlr.isKeyDown(SDLK_d)) {
			body_->ApplyLinearImpulse(pos1, vacio, true);
			//o->getVel().set(o->getVel() + Vector2D(speed_, 0));
		}
	}
}
