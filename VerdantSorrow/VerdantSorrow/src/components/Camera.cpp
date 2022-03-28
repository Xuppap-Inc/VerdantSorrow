#include "Camera.h"
#include "../sdlutils/SDLUtils.h"
#include "Transform.h"
#include "./player/PlayerCtrl.h"
#include "./player/PlayerHubControl.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../sdlutils/SDLUtils.h"

Camera::Camera() : tr_(nullptr), player_(nullptr), followDistance_(0), cameraSpeed_(0), 
	limitLeft_(0), limitTop_(0), followDirections_(false)
{
	limitRight_ = sdlutils().width();
	limitBot_ = sdlutils().height();
}

Camera::Camera(float followDistance, float cameraSpeed, float limitRight, float limitLeft, float limitTop, float limitBot) : tr_(nullptr), player_(nullptr), followDistance_(0), cameraSpeed_(0),
limitRight_(limitRight), limitLeft_(limitLeft), limitTop_(limitTop), limitBot_(limitBot), followDirections_()
{
	followDirections_[0] = limitRight > sdlutils().width();
	followDirections_[1] = limitLeft < 0;
	followDirections_[2] = limitTop < 0;
	followDirections_[3] = limitBot > sdlutils().height();
}

Camera::~Camera()
{
}

void Camera::update()
{
	auto& pos = tr_->getPos();
	pos = player_->getPos() - Vector2D(sdlutils().width()/2, sdlutils().height()/2);
	//calculateDirection();
}

void Camera::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	player_ = mngr_->getHandler(ecs::_PLAYER)->getComponent<Transform>();
	assert(tr_ != nullptr && player_ != nullptr);
}

void Camera::calculateDirection()
{
	auto& vel = tr_->getVel();
	auto& pos = tr_->getPos();
	auto moveVector = player_->getPos() - tr_->getPos();
	auto moveNormalized = moveVector.normalize();
	auto xDistance = moveVector.getX();
	auto yDistance = moveVector.getY();	
	bool stoppingX = abs(xDistance) < followDistance_;
	bool stoppingY = abs(yDistance) < followDistance_;

	//Movimiento hacia la derecha
	if (abs(xDistance) > 0) {

		if ((xDistance > 0 && followDirections_[0] == true) || xDistance < 0 && followDirections_[1] == true) {
			if (abs(xDistance) < 0.1) {
				xDistance = 0;
				pos.setX(player_->getPos().getX());
			}
			stoppingX? vel.setX(xDistance) : vel.setX(moveNormalized.getX() * cameraSpeed_);
		}
	}
	if (abs(yDistance) > 0) {
		//Movimiento hacia arriba
		if ((yDistance < 0 && followDirections_[2] == true) || (yDistance > 0 && followDirections_[3] == true)) {
			if (abs(yDistance) < 0.1) {
				yDistance = 0;
				pos.setY(player_->getPos().getY());
			}
			stoppingY ? vel.setY(xDistance) : vel.setY(moveNormalized.getY() * cameraSpeed_);
		}
	}

	
}


