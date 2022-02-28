#include "PlayerCtrl.h"
#include "../../sdlutils/InputHandler.h"
#include "../../ecs/Entity.h"
#include "../Transform.h"
#include "Attack.h"
#include "../../sdlutils/SDLUtils.h"

using namespace std;
PlayerCtrl::~PlayerCtrl()
{
}

void PlayerCtrl::update()
{
	auto& ihdlr = ih();
	auto currentTime = sdlutils().currRealTime();

	auto& vel = tr_->getVel();
	bool isAttacking = ent_->getComponent<Attack>()->isActive();

	//Si ha pasado el tiempo actual es mayor que cuando se activó el roll + su duración
	//Se desactiva y se activa el deslizar
	if (currentTime >= lastRoll + rollDuration && isRolling) {
		slide = true;
		isRolling = false;
	}


	if (!isAttacking && !isRolling) {
		//std::cout << "canmove" << std::endl;
		if (ihdlr.keyDownEvent()) {
			//std::cout << "key" << std::endl;
			//salto
			if ((ihdlr.isKeyDown(SDLK_w) || ihdlr.isKeyDown(SDLK_SPACE)) && attrib_->isOnGround()) {

				vel.set(Vector2D(vel.getX(), -jumpForce_));
				attrib_->setOnGround(false);
				slide = false;
			}

			//if (ihdlr.isKeyDown(SDLK_a) && ihdlr.isKeyDown(SDLK_d)) {
			//	vel.set(Vector2D(0, vel.getY()));
			//	movementDir_ = 1;
			//	slide = false;
			//	//std::cout << "nulo" << std::endl;
			//}
			
			//movimiento izquierda
			if (ihdlr.isKeyDown(SDLK_a) && !attrib_->isLeftStop()) {

				vel.set(Vector2D(-speed_, vel.getY()));
				movementDir_ = -1;
				slide = false;
				//std::cout << "iz" << std::endl;
			}
			//movimiento derecha
			else if (ihdlr.isKeyDown(SDLK_d) && !attrib_->isRightStop()) {

				vel.set(Vector2D(speed_, vel.getY()));
				movementDir_ = 1;
				slide = false;
				//std::cout << "dr" << std::endl;
			}

			//Roll
			//Si ha pasado el tiempo suficiente como para volver a hacer roll y se pulsa el shift
			//Se activa el cooldown y el booleano que informa que está haciendo el roll
			//Se establece la velocidad
			if (ihdlr.isKeyDown(SDLK_LSHIFT)) {
				if (currentTime >= lastRoll + rollDuration + rollCooldown) {
					vel.set(Vector2D(movementDir_ * rollSpeed_, vel.getY()));
					lastRoll = currentTime;
					isRolling = true;
				}
			}

		}
		else if (ihdlr.isKeyUp(SDLK_a) && ihdlr.isKeyUp(SDLK_d) && ihdlr.isKeyUp(SDLK_w))
			slide = true;
	}

	if (slide)
		doSlide();	

	if (isAttacking)
		doAttack();
}

void PlayerCtrl::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
	attrib_ = ent_->getComponent<PlayerAttributes>();
	assert(attrib_ != nullptr);
	playerCol_ = ent_->getComponent<RectangleCollider>();
	assert(playerCol_ != nullptr);
}

// Realiza un knockback en la direccion especificada
void PlayerCtrl::doKnockback(int dir) {

	tr_->getVel().set(Vector2D(knockbackForceX * dir, -knockbackForceY));
	attrib_->setOnGround(false);

	slide = true;
}

void PlayerCtrl::doAttack()
{
	//Da igual lo que pase si ataca, que va a pararse en seco
	auto& vel = tr_->getVel();
	vel.set(Vector2D(0, vel.getY()));
	slide = false;
}

void PlayerCtrl::doSlide()
{
	//Si deslizar está activado, es decir ha dejado de pulsar d y a
	//Si la velocidad es mayor que 1 se irá reduciendo poco a poco
	//Al llegar a menor de 1 se pondrá a 0 directamente y se desactivará deslizar

	auto vel = tr_->getVel();

	if (abs(vel.getX()) >= 1)
		tr_->getVel().set(Vector2D(vel.getX() * deceleration_, vel.getY()));

	else if (abs(vel.getX()) < 1) {

		tr_->getVel().set(Vector2D(0, vel.getY()));
		slide = false;
	}
}
