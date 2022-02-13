#include "PlayerCtrl.h"
#include "../sdlutils/InputHandler.h"
#include "Container.h"


PlayerCtrl::~PlayerCtrl()
{
}

void PlayerCtrl::handleInput(Container* o)
{
	auto& ihdlr = ih();

	auto& vel = o->getVel();

	if (ihdlr.keyDownEvent()) {
		if (ihdlr.isKeyDown(SDLK_w) && onGround_) {
			
			vel.set(Vector2D(vel.getX(), -jumpForce_));
			onGround_ = false;
		}
		if (ihdlr.isKeyDown(SDLK_a)) {

			vel.set(Vector2D(-speed_, vel.getY()));
		}
		else if (ihdlr.isKeyDown(SDLK_d)) {

			vel.set(Vector2D(speed_, vel.getY()));
		}

		if (ihdlr.isKeyUp(SDLK_a) && ihdlr.isKeyUp(SDLK_d)) vel.set(Vector2D(0, vel.getY()));
	}

	else if (ihdlr.isKeyUp(SDLK_a) && ihdlr.isKeyUp(SDLK_d)) vel.set(Vector2D(0, vel.getY()));
}

bool PlayerCtrl::isOnGround()
{
	return onGround_;
}

void PlayerCtrl::setOnGround(bool set)
{
	onGround_ = set;
}
