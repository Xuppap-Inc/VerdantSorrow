#include "Punietazo.h"
#include "../../../sdlutils/SDLUtils.h"
#include "HandsManager.h"
#include "../../../ecs/Entity.h"
#include "../../../ecs/Manager.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../Transform.h"
#include "../../RectangleCollider.h"



Punietazo::Punietazo():time(sdlutils().currRealTime()),speed(4),tiempo2(sdlutils().currRealTime())
{
}

Punietazo::~Punietazo()
{
}

void Punietazo::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	rect_= ent_->getComponent<RectangleCollider>();
	playertr_ = mngr_->getHandler(ecs::_PLAYER)->getComponent<Transform>();
}

void Punietazo::update()
{
	if (tr_->getPos().getY() <= sdlutils().height() - tr_->getHeight()) {
		tr_->getPos().setY(tr_->getPos().getY() +3);
		
		rect_->setActive(false);
	}
	else {
		auto width = tr_->getWidth();
		auto height = tr_->getHeight();
		auto& vel_ = tr_->getVel();
		if (!para&&!(tr_->getPos().getX() >= playertr_->getPos().getX()  && tr_->getPos().getX() < playertr_->getPos().getX() + playertr_->getWidth())) {
			
			auto mano = tr_->getPos();
			auto player = playertr_->getPos();
			time = sdlutils().currRealTime();
			tiempo2 = sdlutils().currRealTime();
			if (mano.getX() > player.getX()) {
				vel_ = Vector2D(-speed, 0);
			}
			else { 
				vel_ = Vector2D(speed, 0);
			}
		//	vel_ = Vector2D(Vector2D(q - p) > 0 ? 1.0f : -1.0f,0);

		}
		else if(!stop) {
			para = true;
			vel_ = Vector2D(0, 0);
			if (sdlutils().currRealTime() - time >= 750) {
				rect_->setIsTrigger(true);
				rect_->setActive(true);			
				rect_->setWidth(tr_->getWidth());
				rect_->setHeight(tr_->getHeight());
				tiempo2 = sdlutils().currRealTime();
				stop = true;
			}
			else {
				tr_->setWidth(width + 2);
				tr_->setHeight(height + 2);
				tr_->getPos().setY(tr_->getPos().getY() - 2);
				tr_->getPos().setX(tr_->getPos().getX() - 2);
				tiempo2 = sdlutils().currRealTime();
			}
		}
		if (sdlutils().currRealTime() - tiempo2 >= 500) {
			rect_->setActive(false);
			tr_->setWidth(width - 2);
			tr_->setHeight(height - 2);
			tr_->getPos().setY(tr_->getPos().getY() + 2);
			tr_->getPos().setX(tr_->getPos().getX() + 2);
		}
		
	}
	
}
