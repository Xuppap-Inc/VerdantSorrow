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
	jumpKey({ SDL_SCANCODE_W , SDL_SCANCODE_SPACE }),
	jumpButton({ SDL_CONTROLLER_BUTTON_A }),

	// Attack
	//attackKey({ SDL_SCANCODE_J }),
	//attackButton({ SDL_CONTROLLER_BUTTON_B, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER }),

	// Roll
	rollKey({ SDL_SCANCODE_LSHIFT }),
	rollButton({ SDL_CONTROLLER_BUTTON_B , SDL_CONTROLLER_BUTTON_LEFTSHOULDER })
{
}

PlayerCtrl::~PlayerCtrl()
{
}

void PlayerCtrl::update()
{
	auto currentTime = sdlutils().currRealTime();

	auto& vel = tr_->getVel();
	bool isAttacking = ent_->getComponent<Attack>()->isActive();

	//Si ha pasado el tiempo actual es mayor que cuando se activó el roll + su duración
	//Se desactiva y se activa el deslizar
	if (currentTime >= lastRoll_ + rollDuration_ && isRolling_) {
		slide_ = true;
		isRolling_ = false;
	}

	//handle input
	handleInput();

	if (!isAttacking && !isRolling_ && !isKnockback) {

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
		if (anim_->getCurrentAnimation() != "Chica_AtkFloor")
			if (isRolling_) {
				if (anim_->getCurrentAnimation() != "chicaroll") {

					anim_->changeanim(&sdlutils().images().at("chicaroll"), 3, 9, rollDuration_, 25, "chicaroll");
					anim_->repeat(false);

				}
			} else if ((moveRight_ && !moveLeft_) || (!moveRight_ && moveLeft_)) {
				if (anim_->getCurrentAnimation() != "Chica_Run") {
					anim_->repeat(false);
					anim_->changeanim(&sdlutils().images().at("Chica_Run"), 5, 6, 500, 30, "Chica_Run");
				}			
			}
			else {
				if (anim_->getCurrentAnimation() != "Chica_Idle") {
					anim_->repeat(false);
					anim_->changeanim(&sdlutils().images().at("Chica_Idle"), 5, 6, 1500, 30, "Chica_Idle");
				}
			}
	}
}

void PlayerCtrl::handleInput()
{
	auto& ihdlr = ih();

	cout << "Controller X = " << ihdlr.isControllerButtonDown(SDL_CONTROLLER_BUTTON_A) << endl;

	if (ihdlr.keyUpEvent() || ihdlr.controllerUpEvent()) {

		// MOVEMENT
		if (ihdlr.isKeyUp(SDL_SCANCODE_A))
			moveLeft_ = false;
		if (ihdlr.isKeyUp(SDL_SCANCODE_D))
			moveRight_ = false;

		// Jump
		// Keyboard
		int i = 0;
		while (i < jumpKey.size() && !ihdlr.isKeyUp(jumpKey[i])) i++;
		if (i < jumpKey.size()) jump_ = false;
		// Controller
		i = 0;
		while (i < jumpButton.size() && !ihdlr.isControllerButtonUp(jumpButton[i])) i++;
		if (i < jumpButton.size()) jump_ = false;

		// ROLL
		// Keyboard
		i = 0;
		while (i < rollKey.size() && !ihdlr.isKeyUp(rollKey[i])) i++;
		if (i < rollKey.size()) roll_ = false;
		// Controller
		i = 0;
		while (i < rollButton.size() && !ihdlr.isControllerButtonUp(rollButton[i])) i++;
		if (i < rollButton.size()) roll_ = false;
	}
	if (ihdlr.keyDownEvent() || ihdlr.controllerDownEvent()) {

		// MOVEMENT
		if (ihdlr.isKeyDown(SDL_SCANCODE_A))
			moveLeft_ = true;
		if (ihdlr.isKeyDown(SDL_SCANCODE_D))
			moveRight_ = true;

		// JUMP
		// Keyboard
		int i = 0;
		while (i < jumpKey.size() && !ihdlr.isKeyDown(jumpKey[i])) i++;
		if (i < jumpKey.size()) jump_ = true;
		// Controller
		i = 0;
		while (i < jumpButton.size() && !ihdlr.isControllerButtonDown(jumpButton[i])) i++;
		if (i < jumpButton.size()) jump_ = true;

		// ROLL
		// Keyboard
		i = 0;
		while (i < rollKey.size() && !ihdlr.isKeyDown(rollKey[i])) i++;
		if (i < rollKey.size()) roll_ = true;
		// Controller
		i = 0;
		while (i < rollButton.size() && !ihdlr.isControllerButtonDown(rollButton[i])) i++;
		if (i < rollButton.size()) roll_ = true;
	}

	cout << "jump_ = " << jump_;

	//// Mando
	//if (ihdlr.controllerDownEvent()) {
	//	if (ihdlr.isControllerButtonDown(SDL_CONTROLLER_BUTTON_A))
	//		cout << "SDL_CONTROLLER_BUTTON_A" << endl;
	//	if (ihdlr.isControllerButtonDown(SDL_CONTROLLER_BUTTON_B))
	//}

	//// Mando
	//if (ihdlr.isControllerButtonDown(SDL_CONTROLLER_BUTTON_A))
	//	cout << "SDL_CONTROLLER_BUTTON_A" << endl;
	////else (ihdlr.isControllerButtonDown(SDL_CONTROLLER_BUTTON_A))
}

void PlayerCtrl::disableKnockback()
{
	if (abs(tr_->getVel().getX()) < 1)
		isKnockback = false;
}
