#include "PlayerCtrl.h"
#include "../sdlutils/InputHandler.h"
#include "../ecs/Entity.h"
#include "Transform.h"


PlayerCtrl::~PlayerCtrl()
{
}

void PlayerCtrl::update()
{
	auto& ihdlr = ih();

	auto& vel = tr_->getVel();

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

void PlayerCtrl::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
}

bool PlayerCtrl::isOnGround()
{
	return onGround_;
}

void PlayerCtrl::setOnGround(bool set)
{
	onGround_ = set;
}
