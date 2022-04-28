#include "PlayerHubControl.h"
#include "../../sdlutils/InputHandler.h"
#include "../../ecs/Entity.h"
#include "../Transform.h"
#include "../../sdlutils/SDLUtils.h"
#include "../../ecs/Manager.h"
#include "../hub/NpcCtrl.h"
using namespace std;
PlayerHubControl::PlayerHubControl(float speed, CollisionManager* colManager) : playerCol_(), tr_(), speed_(speed), attrib_(), colMan_(colManager),
moveDown_(false), moveLeft_(false), moveRight_(false), moveUp_(false), isTalking_(false)
{
}

void PlayerHubControl::update()
{
	

	auto& vel = tr_->getVel();


	handleInput();

	//movimiento en 8 direcciones
	vel.set(Vector2D(0, 0));
	if (!isTalking_) {
		if (moveUp_ && !moveDown_)
			vel.setY(-speed_);
		else if (!moveUp_ && moveDown_)
			vel.setY(speed_);

		if (moveLeft_ && !moveRight_)
			vel.setX(-speed_);
		else if (!moveLeft_ && moveRight_)
			vel.setX(speed_);

		if (vel.magnitude() != 0)
			vel = vel.normalize() * speed_;

		if (colMan_->hasCollisions(playerCol_)) {
			std::vector<RectangleCollider*> colliders = colMan_->getCollisions(playerCol_);

			bool changeScene = false;
			int i = 0;
			while (!changeScene && i < colliders.size()) {
				changeScene = colliders[i]->isActive() && colliders[i]->isTrigger() && colliders[i]->getEntity()->getComponent<NpcCtrl>() == nullptr;
				i++;
			}
			//if (changeScene)
				//mngr_->changeScene(1);
		}
	}
}

void PlayerHubControl::initComponent()
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

void PlayerHubControl::handleInput()
{
	auto& ihdlr = ih();

	if (ihdlr.keyUpEvent()) {
		if (ihdlr.isKeyUp(SDL_SCANCODE_A))
			moveLeft_ = false;
		if (ihdlr.isKeyUp(SDL_SCANCODE_D))
			moveRight_ = false;
		if (ihdlr.isKeyUp(SDL_SCANCODE_W))
			moveUp_ = false;
		if (ihdlr.isKeyUp(SDL_SCANCODE_S))
			moveDown_ = false;
	}
	if (ihdlr.keyDownEvent()) {
		if (ihdlr.isKeyDown(SDL_SCANCODE_A))
			moveLeft_ = true;
		if (ihdlr.isKeyDown(SDL_SCANCODE_D))
			moveRight_ = true;
		if (ihdlr.isKeyDown(SDL_SCANCODE_W))
			moveUp_ = true;
		if (ihdlr.isKeyDown(SDL_SCANCODE_S))
			moveDown_ = true;
	}
	//Movimiento joystick
	if (ihdlr.controllerConnected()) {

		float axisValueX = ihdlr.getAxisValue(SDL_CONTROLLER_AXIS_LEFTX);
		float axisValueY = ihdlr.getAxisValue(SDL_CONTROLLER_AXIS_LEFTY);

		//Movmiento x
		if (axisValueX < -.3f)
			moveLeft_ = true;
		if (axisValueX > .3f)
			moveRight_ = true;

		if (axisValueX < .3f && axisValueX > -.3f)
		{
			moveLeft_ = false; moveRight_ = false;
		}

		//Movimiento y
		if (axisValueY < -.3f)
			moveUp_ = true;
		if (axisValueY > .3f)
			moveDown_ = true;

		if (axisValueY < .3f && axisValueY > -.3f)
		{
			moveUp_ = false; moveDown_ = false;
		}
	}



	if (moveLeft_)
		anim_->flipX(true);
	else
		anim_->flipX(false);


	if (moveDown_) {
		anim_->changeanim(&sdlutils().imagesHub().at("walk_Kyna"), 3, 9, (1000 / 25) * 25, 25, "walk_Kyna");
	}

	else if (moveUp_) {
		anim_->changeanim(&sdlutils().imagesHub().at("walk_up_Kyna"), 3, 9, (1000 / 25) * 25, 25, "walk_side_Kyna");
	}

	else if (moveRight_) {
		anim_->changeanim(&sdlutils().imagesHub().at("walk_side_Kyna"), 3, 9, (1000 / 25) * 25, 25, "walk_side_Kyna");
	}

	else if (moveLeft_) {
		anim_->changeanim(&sdlutils().imagesHub().at("walk_side_Kyna"), 3, 9, (1000 / 25) * 25, 25, "walk_side_Kyna");
	}

	else if (!moveUp_ && !moveDown_ && !moveLeft_ && !moveRight_) {
		anim_->changeanim(&sdlutils().imagesHub().at("idle_Kyna"), 4, 8, (1000 / 30) * 30, 30, "idle_Kyna");
	}
}
