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
		if (ihdlr.isKeyDown(SDLK_w) && attrib_->isOnGround()) {
			
			vel.set(Vector2D(vel.getX(), -jumpForce_));
			attrib_->setOnGround(false);
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
	
	attrib_ = ent_->getComponent<PlayerAttributes>();
	assert(attrib_ != nullptr);
}
