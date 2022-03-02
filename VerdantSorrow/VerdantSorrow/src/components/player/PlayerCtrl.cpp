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
	rollCooldown_(1000), rollDuration_(500), isRolling_(false), knockbackForceX_(10), knockbackForceY_(10), slide_(false), roll_(false)
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

	if (!isAttacking && !isRolling_) {

		//handle input
		handleInput();

		//salto
		if (jump_ && attrib_->isOnGround()) {

			vel.set(Vector2D(vel.getX(), -jumpForce_));
			attrib_->setOnGround(false);
			slide_ = false;
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
		}
		//movimiento derecha
		else if (moveRight_ && !attrib_->isRightStop()) {

			vel.set(Vector2D(speed_, vel.getY()));
			movementDir_ = 1;
			slide_ = false;
		}

		//Roll
		if (roll_ && currentTime >= lastRoll_ + rollDuration_ + rollCooldown_) {
			vel.set(Vector2D(movementDir_ * rollSpeed_, vel.getY()));
			lastRoll_ = currentTime;
			isRolling_ = true;
		}
			
		if (!jump_ && !moveLeft_ && !moveRight_)
			slide_ = true;
	}

	if (slide_)
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

	tr_->getVel().set(Vector2D(knockbackForceX_ * dir, -knockbackForceY_));
	attrib_->setOnGround(false);

	slide_ = true;
}

void PlayerCtrl::doAttack()
{
	//Da igual lo que pase si ataca, que va a pararse en seco
	auto& vel = tr_->getVel();
	vel.set(Vector2D(0, 0));
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

void PlayerCtrl::handleInput()
{
	auto& ihdlr = ih();

	if (ihdlr.keyUpEvent()) {
		if (ihdlr.isKeyUp(SDL_SCANCODE_A))
			moveLeft_ = false;
		if (ihdlr.isKeyUp(SDL_SCANCODE_D))
			moveRight_ = false;
		if (ihdlr.isKeyUp(SDL_SCANCODE_W))
			jump_ = false;
		if (ihdlr.isKeyUp(SDL_SCANCODE_SPACE))
			jump_ = false;
		if (ihdlr.isKeyUp(SDL_SCANCODE_LSHIFT))
			roll_ = false;
	}
	if (ihdlr.keyDownEvent()) {
		if (ihdlr.isKeyDown(SDL_SCANCODE_A))
			moveLeft_ = true;
		if (ihdlr.isKeyDown(SDL_SCANCODE_D))
			moveRight_ = true;
		if (ihdlr.isKeyDown(SDL_SCANCODE_W))
			jump_ = true;
		if (ihdlr.isKeyDown(SDL_SCANCODE_SPACE))
			jump_ = true;
		if (ihdlr.isKeyDown(SDL_SCANCODE_LSHIFT))
			roll_ = true;
	}
}
