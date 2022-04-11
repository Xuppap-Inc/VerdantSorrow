#include "ScrollCamera.h"
#include "../sdlutils/SDLUtils.h"
#include "Transform.h"
#include "./player/PlayerCtrl.h"
#include "./player/PlayerHubControl.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../sdlutils/SDLUtils.h"

ScrollCamera::ScrollCamera() : tr_(nullptr), player_(nullptr), cameraSpeed_(0), deadzoneX_(), deadzoneY_(),scrollX_(false)
{
	
}

ScrollCamera::ScrollCamera( float cameraSpeed, float deadzoneX, float deadzoneY) : 
	tr_(nullptr), player_(nullptr),  cameraSpeed_(cameraSpeed), deadzoneX_(deadzoneX), deadzoneY_(deadzoneY)
{

}

ScrollCamera::~ScrollCamera()
{
}

void ScrollCamera::update()
{
	//auto& pos = tr_->getPos();
	//pos = player_->getPos() - Vector2D(sdlutils().width()/2, sdlutils().height()/2);
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

	//Movimiento en X
	if (diff.getX() > deadzoneX_) {
	
		vel.setX(cameraSpeed_);
	}
	if (diff.getX() + player_->getWidth() < 0) {
		vel.setX(-cameraSpeed_);
	}
	else if (diff.getX() <= deadzoneX_ && diff.getX() + player_->getWidth() >= 0) {
		vel.setX(vel.getX()*0.995);
		if (abs(diff.getX()) < 10 && abs(diff.getX())>0) {
			vel.setX(0);
		}
	}

	if (!scrollX_) {
		//Movimiento en Y
		if (diff.getY() > deadzoneY_) {

			vel.setY(cameraSpeed_);
		}
		if (diff.getY() + player_->getHeight() < 0) {
			vel.setY(-cameraSpeed_);
		}
		else if (diff.getY() <= deadzoneY_ && diff.getX() + player_->getHeight() >= 0) {
			vel.setY(vel.getY() * 0.995);
			if (abs(diff.getY()) < 10 && abs(diff.getY()) > 0) {
				vel.setY(0);
			}
		}

	}
	
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


