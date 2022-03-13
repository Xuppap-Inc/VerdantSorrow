#include "Punietazo.h"
#include "../../../sdlutils/SDLUtils.h"
#include "HandsManager.h"
#include "../../../ecs/Entity.h"
#include "../../../ecs/Manager.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../Transform.h"
#include "../../RectangleCollider.h"



Punietazo::Punietazo() :time(sdlutils().currRealTime()), speed(8), tiempo2(sdlutils().currRealTime()), state_(REPOSO)
{

}

Punietazo::~Punietazo()
{
}

void Punietazo::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
	col_ = ent_->getComponent<RectangleCollider>();
	assert(col_ != nullptr);
	playertr_ = mngr_->getHandler(ecs::_PLAYER)->getComponent<Transform>();
	assert(playertr_ != nullptr);

	initialwidth_ = tr_->getWidth();
	initialheight_ = tr_->getHeight();
}

void Punietazo::goDown()
{
	if (tr_->getPos().getY() <= sdlutils().height() - tr_->getHeight()) {
		tr_->getPos().setY(tr_->getPos().getY() + 3);
		col_->setActive(false);
	}
	else
		state_ = FOLLOW;
}

void Punietazo::followPlayer()
{
	auto width = tr_->getWidth();
	auto height = tr_->getHeight();
	auto& vel_ = tr_->getVel();

	if (!(tr_->getPos().getX() >= playertr_->getPos().getX() && tr_->getPos().getX() < playertr_->getPos().getX() + playertr_->getWidth())) {

		auto mano = tr_->getPos();
		auto player = playertr_->getPos();

		if (mano.getX() > player.getX()) {
			vel_ = Vector2D(-speed, 0);
		}
		else {
			vel_ = Vector2D(speed, 0);
		}
	}
	else
	{
		time = sdlutils().currRealTime();
		state_ = HIT;
	}
}

void Punietazo::hit()
{
	auto width = tr_->getWidth();
	auto height = tr_->getHeight();
	auto& vel_ = tr_->getVel();
	vel_ = Vector2D(0, 0);

	if (sdlutils().currRealTime() - time >= 350) {
		col_->setIsTrigger(true);
		col_->setActive(true);
		col_->setWidth(tr_->getWidth());
		col_->setHeight(tr_->getHeight());
		state_ = REDUCE;
		tiempo2 = sdlutils().currRealTime();
	}
	else {
		tr_->setWidth(width + 4);
		tr_->setHeight(height + 4);
		tr_->getPos().setY(tr_->getPos().getY() - 4);
		tr_->getPos().setX(tr_->getPos().getX() - 4);
	}
}

void Punietazo::reduce()
{
	auto width = tr_->getWidth();
	auto height = tr_->getHeight();

	if (sdlutils().currRealTime() - tiempo2 >= 750) {
		if (width > initialwidth_) {
			col_->setActive(false);
			tr_->setWidth(width - 2);
			tr_->setHeight(height - 2);
			tr_->getPos().setY(tr_->getPos().getY() + 2);
			tr_->getPos().setX(tr_->getPos().getX() + 2);

		}
		else
			state_ = FIN;

	}

}


