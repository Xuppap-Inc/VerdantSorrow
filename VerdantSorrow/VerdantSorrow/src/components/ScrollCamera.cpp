#include "ScrollCamera.h"
#include "../sdlutils/SDLUtils.h"
#include "Transform.h"
#include "./player/PlayerCtrl.h"
#include "./player/PlayerHubControl.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../sdlutils/SDLUtils.h"

ScrollCamera::ScrollCamera() : tr_(nullptr), player_(nullptr), cameraSpeed_(0), deadzoneX_(), deadzoneY_(),scrollX_(true), scrollY_(true),
		limitRight_(INT_MAX), limitLeft_(INT_MIN), limitBot_(INT_MAX), limitTop_(INT_MIN)
{
	
}

ScrollCamera::ScrollCamera( float cameraSpeed, float deadzoneX, float deadzoneY) : 
	tr_(nullptr), player_(nullptr),  cameraSpeed_(cameraSpeed), deadzoneX_(deadzoneX), deadzoneY_(deadzoneY), scrollX_(true), scrollY_(true),
	limitRight_(INT_MAX), limitLeft_(INT_MIN), limitBot_(INT_MAX), limitTop_(INT_MIN)
{

}

ScrollCamera::~ScrollCamera()
{
}

void ScrollCamera::update()
{
	calculateDirection();
}

void ScrollCamera::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	player_ = mngr_->getHandler(ecs::_PLAYER)->getComponent<Transform>();
	assert(tr_ != nullptr && player_ != nullptr);
}

void ScrollCamera::calculateDirection()
{
	auto& pos = tr_->getPos();
	auto actPos = pos + Vector2D(sdlutils().width() / 2.0 - deadzoneX_ / 2.0, sdlutils().height() / 2.0 - deadzoneY_ / 2.0);
	auto& vel = tr_->getVel();
	auto playerPos = player_->getPos();
	auto diff = playerPos - actPos;

	if (pos.getX() < limitLeft_ || pos.getX() + sdlutils().width() > limitRight_) {
		scrollX_ = false;
		vel.setX(0);
		pos.setX(pos.getX() <= limitLeft_ ? limitLeft_ : limitRight_ - sdlutils().width());
	}
	else scrollX_ = true;

	if (pos.getY() < limitTop_ || pos.getY() + sdlutils().height() > limitBot_) {
		scrollY_ = false;
		vel.setY(0);
		pos.setY(pos.getY() <= limitTop_ ? limitTop_ : limitBot_ - sdlutils().height());
	}
	else scrollY_ = true;
	


	if (scrollX_) {
		//Movimiento en X
		if (diff.getX() > deadzoneX_) {

			vel.setX(cameraSpeed_);
		}
		if (diff.getX() + player_->getWidth() < 0) {
			vel.setX(-cameraSpeed_);
		}
		else if (diff.getX() <= deadzoneX_ && diff.getX() + player_->getWidth() >= 0) {
			vel.setX(vel.getX() * 0.995);
			if (abs(diff.getX()) < 10 && abs(diff.getX()) > 0) {
				vel.setX(0);
			}
		}
	}

	if (scrollY_) {
		//Movimiento en Y
		if (diff.getY() > deadzoneY_) {

			vel.setY(cameraSpeed_);
		}
		if (diff.getY() + player_->getHeight() < 0) {
			vel.setY(-cameraSpeed_);
		}
		else if (diff.getY() <= deadzoneY_ && diff.getY() + player_->getHeight() >= 0) {
			vel.setY(vel.getY() * 0.995);
			if (abs(diff.getY()) < 10 && abs(diff.getY()) > 0) {
				vel.setY(0);
			}
		}
	}

	if (vel.getX() >= cameraSpeed_ && vel.getY() <= cameraSpeed_) vel = vel.normalize() * cameraSpeed_;
	
}

void ScrollCamera::debug() 
{
	auto pos = Vector2D(sdlutils().width() / 2.0 - deadzoneX_/2.0, sdlutils().height() / 2.0 - deadzoneY_/2.0);
	auto dest = build_sdlrect(pos, deadzoneX_, deadzoneY_);

	//escalado pantalla
	auto sW = mngr_->getWindowScaleWidth();
	auto sH = mngr_->getWindowScaleHeight();

	dest.x *= sW;
	dest.w *= sW;
	dest.y *= sH;
	dest.h *= sH;

	SDL_RenderDrawRect(sdlutils().renderer(), &dest);
}


