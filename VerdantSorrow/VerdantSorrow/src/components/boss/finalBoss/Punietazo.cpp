#include "Punietazo.h"
#include "../../../sdlutils/SDLUtils.h"
#include "HandsManager.h"
#include "../../../ecs/Entity.h"
#include "../../../ecs/Manager.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../Transform.h"
#include "../../RectangleCollider.h"



Punietazo::Punietazo(State state):time(sdlutils().currRealTime()),speed(8),tiempo2(sdlutils().currRealTime()),state_(state)
{

}

Punietazo::~Punietazo()
{
}

void Punietazo::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	rect_ = ent_->getComponent<RectangleCollider>();
	playertr_ = mngr_->getHandler(ecs::_PLAYER)->getComponent<Transform>();
	initialwidth_ = tr_->getWidth();
	initialheight_ = tr_->getHeight();
}

void Punietazo::godown()
{
	
	if (tr_->getPos().getY() <= sdlutils().height() - tr_->getHeight()) {
		tr_->getPos().setY(tr_->getPos().getY() + 3);
		rect_->setActive(false);
	}
	else  state_ = FOLLOW; 
	

	
	
}

void Punietazo::persiguejugador()
{
	auto width = tr_->getWidth();
	auto height = tr_->getHeight();
	auto& vel_ = tr_->getVel();
	if (!(tr_->getPos().getX() >= playertr_->getPos().getX() && tr_->getPos().getX() < playertr_->getPos().getX() + playertr_->getWidth())) {

		auto mano = tr_->getPos();
		auto player = playertr_->getPos();
		//time = sdlutils().currRealTime();
		//tiempo2 = sdlutils().currRealTime();
		if (mano.getX() > player.getX()) {
			vel_ = Vector2D(-speed, 0);
		}
		else {
			vel_ = Vector2D(speed, 0);
		}
		//	vel_ = Vector2D(Vector2D(q - p) > 0 ? 1.0f : -1.0f,0);
	}
	else
	{
		time = sdlutils().currRealTime();
		state_ = PUNIETAZO;
	}
}

void Punietazo::punietazo()
{
	auto width = tr_->getWidth();
	auto height = tr_->getHeight();
	auto& vel_ = tr_->getVel();
	vel_ = Vector2D(0, 0);
	if (sdlutils().currRealTime() - time >= 350) {
		rect_->setIsTrigger(true);
		rect_->setActive(true);
		rect_->setWidth(tr_->getWidth());
		rect_->setHeight(tr_->getHeight());		
		state_ =FUERA;
		tiempo2 = sdlutils().currRealTime();
	}
	else {
		tr_->setWidth(width + 4);
		tr_->setHeight(height + 4);
		tr_->getPos().setY(tr_->getPos().getY() - 4);
		tr_->getPos().setX(tr_->getPos().getX() - 4);		
	}
}

void Punietazo::makesamall()
{
	auto width = tr_->getWidth();
	auto height = tr_->getHeight();
	if (sdlutils().currRealTime() - tiempo2 >= 750) {
		if (width > initialwidth_) {
			rect_->setActive(false);
			tr_->setWidth(width - 2);
			tr_->setHeight(height - 2);
			tr_->getPos().setY(tr_->getPos().getY() + 2);
			tr_->getPos().setX(tr_->getPos().getX() + 2);

		}
		else {
			state_ = FIN;
		}
	}
	
}

void Punietazo::update()
{
	

		switch (state_) {
		case  State::DOWN:
			godown();
			break;
		case  State::FOLLOW:
			persiguejugador();
			break;
		case  State::PUNIETAZO:
			punietazo();
			break;
		case  State::FUERA:
			makesamall();
			
			
			break;
		default:
			break;
		}
	


	
}


