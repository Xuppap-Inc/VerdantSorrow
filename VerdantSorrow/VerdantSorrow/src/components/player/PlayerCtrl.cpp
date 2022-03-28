#include "PlayerCtrl.h"
#include "../../sdlutils/InputHandler.h"
#include "../../ecs/Entity.h"
#include "../Transform.h"
#include "Attack.h"
#include "../../sdlutils/SDLUtils.h"

using namespace std;
PlayerCtrl::PlayerCtrl(float jumpForce, float speed, float deceleration, float rollSpeed) :
	tr_(nullptr), speed_(speed), jumpForce_(jumpForce), rollSpeed_(rollSpeed), deceleration_(deceleration),
	attrib_(), movementDir_(1), lastRoll_(), playerCol_(nullptr), moveLeft_(false), moveRight_(false), jump_(false),
	rollCooldown_(100), rollDuration_(500), isRolling_(false), knockbackForceX_(40), knockbackForceY_(10), slide_(false), roll_(false)
	, isKnockback(false),

	// INPUT
	// Jump
	jumpKeys({ SDL_SCANCODE_W , SDL_SCANCODE_SPACE }),
	jumpButtons({ SDL_CONTROLLER_BUTTON_A }),

	// Roll
	rollKeys({ SDL_SCANCODE_LSHIFT }),
	rollButtons({ SDL_CONTROLLER_BUTTON_B , SDL_CONTROLLER_BUTTON_LEFTSHOULDER })
{
}

PlayerCtrl::~PlayerCtrl()
{
}

void PlayerCtrl::update()
{
	auto currentTime = sdlutils().currRealTime();

	auto& vel = tr_->getVel();
	bool isAttacking = attack_->isActive();

	//Si ha pasado el tiempo actual es mayor que cuando se activó el roll + su duración
	//Se desactiva y se activa el deslizar
	if (currentTime >= lastRoll_ + rollDuration_ && isRolling_) {
		slide_ = true;
		isRolling_ = false;
	}

	//handle input
	handleInput();

	//!isAttacking
	if ( !isRolling_ && !isKnockback) {

		//salto
		if (jump_ && attrib_->isOnGround()) {

			vel.set(Vector2D(vel.getX(), -jumpForce_));
			attrib_->setOnGround(false);
			slide_ = false;

			// Animacion
			anim_->repeat(true);
			anim_->changeanim(&sdlutils().images().at("Chica_Jump"), 4, 5, 300, 20, "Chica_Jump");
		}

		//moviemiento nulo
		if (moveRight_ && moveLeft_) {
			vel.set(Vector2D(0, vel.getY()));
			movementDir_ = 1;
			slide_ = false;
		}
		//movimiento izquierda
		else if (moveLeft_ && !attrib_->isLeftStop()) {

			vel.set(Vector2D(-speed_, vel.getY()));
			movementDir_ = -1;
			slide_ = false;

			anim_->flipX(true);
		}
		//movimiento derecha
		else if (moveRight_ && !attrib_->isRightStop()) {

			vel.set(Vector2D(speed_, vel.getY()));
			movementDir_ = 1;
			slide_ = false;

			anim_->flipX(false);
		}
		else
			slide_ = true;

		//Roll
		if (roll_ && currentTime >= lastRoll_ + rollDuration_ + rollCooldown_) {
			vel.set(Vector2D(movementDir_ * rollSpeed_, vel.getY()));
			lastRoll_ = currentTime;
			isRolling_ = true;
			slide_ = false;
		}	

	}

	animationManagement();

	if (slide_)
		doSlide();

	if (isAttacking)
		doAttack();

	if (isKnockback)
		disableKnockback();

}

void PlayerCtrl::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
	attrib_ = ent_->getComponent<PlayerAttributes>();
	assert(attrib_ != nullptr);
	playerCol_ = ent_->getComponent<RectangleCollider>();
	assert(playerCol_ != nullptr);
	anim_ = ent_->getComponent<FramedImage>();
	assert(anim_ != nullptr);
	attack_ = ent_->getComponent<Attack>();
	assert(attack_ != nullptr);
}

// Realiza un knockback en la direccion especificada
void PlayerCtrl::doKnockback(int dir) {

	tr_->getVel().set(Vector2D(knockbackForceX_ * dir, -knockbackForceY_));
	attrib_->setOnGround(false);

	isKnockback = true;
	slide_ = true;
}

void PlayerCtrl::doAttack()
{
	//Da igual lo que pase si ataca, que va a pararse en seco
	auto& vel = tr_->getVel();
	vel.set(Vector2D(0, vel.getY()));
	slide_ = false;
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
		slide_ = false;
	}
}

void PlayerCtrl::animationManagement()
{
	// Animation
	if (attrib_->isOnGround()) {
		if (anim_->getCurrentAnimation() != "Chica_AtkFloor" || attack_->hasFinished())
			if (isRolling_) {
				if (anim_->getCurrentAnimation() != "chicaroll") {

					if (anim_->getCurrentAnimation() == "Chica_AtkFloor" || anim_->getCurrentAnimation() == "Attack1_Recovery") attack_->deactivateRecovery();

					anim_->changeanim(&sdlutils().images().at("chicaroll"), 3, 9, rollDuration_, 25, "chicaroll");
					anim_->repeat(false);
				}
			}
			else if ((moveRight_ && !moveLeft_) || (!moveRight_ && moveLeft_)) {

				if (anim_->getCurrentAnimation() == "Chica_AtkFloor" || anim_->getCurrentAnimation() == "Attack1_Recovery") attack_->deactivateRecovery();

				if (anim_->getCurrentAnimation() != "Chica_Run") {
					anim_->repeat(true);
					anim_->changeanim(&sdlutils().images().at("Chica_Run"), 5, 6, 500, 30, "Chica_Run");
				}
			}
			else if (attack_->hasFinishedRecovery()) {
				if (anim_->getCurrentAnimation() != "Chica_Idle" ) {
					anim_->repeat(true);
					anim_->changeanim(&sdlutils().images().at("Chica_Idle"), 5, 6, 1500, 30, "Chica_Idle");
				}
			}
	}
}

void PlayerCtrl::handleInput()
{
	auto& ihdlr = ih();

	// BUTTON UP
	if (ihdlr.keyUpEvent() || ihdlr.controllerUpEvent()) {

		// KEYBOARD
		if (!ihdlr.controllerConnected()) {

			// MOVEMENT
			if (ihdlr.isKeyUp(SDL_SCANCODE_A))
				moveLeft_ = false;
			if (ihdlr.isKeyUp(SDL_SCANCODE_D))
				moveRight_ = false;

			// JUMP
			int i = 0;
			while (i < jumpKeys.size() && !ihdlr.isKeyUp(jumpKeys[i])) i++;
			if (i < jumpKeys.size()) jump_ = false;

			// ROLL
			i = 0;
			while (i < rollKeys.size() && !ihdlr.isKeyUp(rollKeys[i])) i++;
			if (i < rollKeys.size()) roll_ = false;
		}

		// CONTROLLER
		else {

			// JUMP
			int i = 0;
			while (i < jumpButtons.size() && !ihdlr.isControllerButtonUp(jumpButtons[i])) i++;
			if (i < jumpButtons.size()) jump_ = false;

			// ROLL
			i = 0;
			while (i < rollButtons.size() && !ihdlr.isControllerButtonUp(rollButtons[i])) i++;
			if (i < rollButtons.size()) roll_ = false;
		}
	}

	// BUTTON DOWN
	if (ihdlr.keyDownEvent() || ihdlr.controllerDownEvent()) {

		// KEYBOARD
		if (!ihdlr.controllerConnected()) {
			// MOVEMENT
			if (ihdlr.isKeyDown(SDL_SCANCODE_A))
				moveLeft_ = true;
			if (ihdlr.isKeyDown(SDL_SCANCODE_D))
				moveRight_ = true;

			// JUMP
			int i = 0;
			while (i < jumpKeys.size() && !ihdlr.isKeyDown(jumpKeys[i])) i++;
			if (i < jumpKeys.size()) jump_ = true;

			// ROLL
			i = 0;
			while (i < rollKeys.size() && !ihdlr.isKeyDown(rollKeys[i])) i++;
			if (i < rollKeys.size()) roll_ = true;
		}

		// CONTROLLER
		else {
			// JUMP
			int i = 0;
			while (i < jumpButtons.size() && !ihdlr.isControllerButtonDown(jumpButtons[i])) i++;
			if (i < jumpButtons.size()) jump_ = true;

			// ROLL
			i = 0;
			while (i < rollButtons.size() && !ihdlr.isControllerButtonDown(rollButtons[i])) i++;
			if (i < rollButtons.size()) roll_ = true;
		}
	}

	// JOYSTICK MOVEMENT
	if (ihdlr.controllerConnected()) {

		float axisValue = ihdlr.getAxisValue(SDL_CONTROLLER_AXIS_LEFTX);

		if (axisValue < -.3f)
			moveLeft_ = true;
		if (axisValue > .3f)
			moveRight_ = true;

		if (axisValue < .3f && axisValue > -.3f)
		{
			moveLeft_ = false; moveRight_ = false;
		}
	}
}

void PlayerCtrl::disableKnockback()
{
	if (abs(tr_->getVel().getX()) < 1)
		isKnockback = false;
}
