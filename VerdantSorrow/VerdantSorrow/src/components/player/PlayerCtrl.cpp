#include "PlayerCtrl.h"
#include "../../sdlutils/InputHandler.h"
#include "../../ecs/Entity.h"
#include "../Transform.h"
#include "Attack.h"


PlayerCtrl::~PlayerCtrl()
{
}

void PlayerCtrl::update()
{
	auto& ihdlr = ih();

	auto& vel = tr_->getVel();
	bool isAttacking = ent_->getComponent<Attack>()->isActive();

	//el jugador solo se puede mover si no esta atacando
	if (!isAttacking && ihdlr.keyDownEvent()) {
		
		//salto
		if (ihdlr.isKeyDown(SDLK_w) && attrib_->isOnGround()) {
			
			vel.set(Vector2D(vel.getX(), -jumpForce_));
			attrib_->setOnGround(false);
		}
		//movimiento izquierda
		if (ihdlr.isKeyDown(SDLK_a) && !attrib_->isLeftStop()) {

			vel.set(Vector2D(-speed_, vel.getY()));
			movementDir_ = -1;
		}
		//movimiento derecha
		else if (ihdlr.isKeyDown(SDLK_d) && !attrib_->isRightStop()) {

			vel.set(Vector2D(speed_, vel.getY()));
			movementDir_ = 1;
		}

		//si no se están pulsando las de movimiento se queda quieto
		if (ihdlr.isKeyUp(SDLK_a) && ihdlr.isKeyUp(SDLK_d)) vel.set(Vector2D(0, vel.getY()));
	}

	//si no se están pulsando las de movimiento se queda quieto
	else if (isAttacking || (ihdlr.isKeyUp(SDLK_a) && ihdlr.isKeyUp(SDLK_d))) vel.set(Vector2D(0, vel.getY()));
}

void PlayerCtrl::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
	
	attrib_ = ent_->getComponent<PlayerAttributes>();
	assert(attrib_ != nullptr);
}
