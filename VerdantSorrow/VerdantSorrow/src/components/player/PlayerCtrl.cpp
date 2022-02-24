#include "PlayerCtrl.h"
#include "../../sdlutils/InputHandler.h"
#include "../../ecs/Entity.h"
#include "../Transform.h"
#include "Attack.h"

using namespace std;
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
		if ((ihdlr.isKeyDown(SDLK_w) || ihdlr.isKeyDown(SDLK_SPACE)) && attrib_->isOnGround()) {
			
			vel.set(Vector2D(vel.getX(), -jumpForce_));
			attrib_->setOnGround(false);
			deslizar = false;
		}
		//movimiento izquierda
		if (ihdlr.isKeyDown(SDLK_a) && !attrib_->isLeftStop()) {
			 
			vel.set(Vector2D(-speed_, vel.getY()));
			movementDir_ = -1;
			deslizar = false;
		}
		//movimiento derecha
		else if (ihdlr.isKeyDown(SDLK_d) && !attrib_->isRightStop()) {

			vel.set(Vector2D(speed_, vel.getY()));
			movementDir_ = 1;
			deslizar = false;
		}
		//Roll
		if (ihdlr.isKeyDown(SDLK_LSHIFT)) vel.set(Vector2D(-speed_, vel.getY()));
	}
	else if (ihdlr.isKeyUp(SDLK_a) && ihdlr.isKeyUp(SDLK_d) && ihdlr.isKeyUp(SDLK_w)) 	
		deslizar = true;
	//Si deslizar está activado, es decir ha dejado de pulsar d y a
	//Si la velocidad es mayor que 1, positiva o negativa se irá reduciendo poco a poco
	if ((tr_->getVel().getX() >= 1 && movementDir_ == 1) || (tr_->getVel().getX() <= -1 && movementDir_ == -1)){
		if (deslizar)
		vel.set(Vector2D(tr_->getVel().getX() * deceleration_, vel.getY()));
	}
		
	
	//Al llegar a menor de 1 se pondrá a 0 directamente y se desactivará deslizar
	else if (((tr_->getVel().getX() < 1 && movementDir_ == 1) || (tr_->getVel().getX() > -1 && movementDir_ == -1)) 
		&& deslizar && attrib_->isOnGround()) {
		vel.set(Vector2D(0, vel.getY()));
		deslizar = false;
	}
	//Da igual lo que pase si ataca, que va a pararse en seco
	if (isAttacking){
		vel.set(Vector2D(0, vel.getY()));
		deslizar = false;
	}
}

void PlayerCtrl::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
	
	attrib_ = ent_->getComponent<PlayerAttributes>();
	assert(attrib_ != nullptr);
}
