#include "PlayerHubControl.h"
#include "../../sdlutils/InputHandler.h"
#include "../../ecs/Entity.h"
#include "../Transform.h"
#include "../../sdlutils/SDLUtils.h"
#include "../../ecs/Manager.h"
#include "../hub/NpcCtrl.h"
using namespace std;
PlayerHubControl::PlayerHubControl(float speed, CollisionManager* colManager) : playerCol_(), tr_(), speed_(speed), attrib_(), colMan_(colManager),
moveDown_(false), moveLeft_(false), moveRight_(false), moveUp_(false)
{
}

void PlayerHubControl::update()
{
	auto currentTime = sdlutils().currRealTime();
	auto& vel = tr_->getVel();


	handleInput();

	//movimiento en 8 direcciones
	vel.set(Vector2D(0, 0));

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

void PlayerHubControl::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);

	attrib_ = ent_->getComponent<PlayerAttributes>();
	assert(attrib_ != nullptr);
	playerCol_ = ent_->getComponent<RectangleCollider>();
	assert(playerCol_ != nullptr);
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
}
