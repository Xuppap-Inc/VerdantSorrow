#include "Punch.h"
#include "../../../sdlutils/SDLUtils.h"
#include "HandsManager.h"
#include "../../../ecs/Entity.h"
#include "../../../ecs/Manager.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../Transform.h"
#include "../../RectangleCollider.h"
#include "../../Image.h"



Punch::Punch(bool manoDerecha) : hitTime_(), goBackTime_(), state_(REPOSO), handSpeed_(8), dW(4), dH(4), manoDerecha_(manoDerecha)
{

}

Punch::~Punch()
{
}

void Punch::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
	col_ = ent_->getComponent<RectangleCollider>();
	assert(col_ != nullptr);
	img_ = ent_->getComponent<Image>();
	playertr_ = mngr_->getHandler(ecs::_PLAYER)->getComponent<Transform>();
	assert(playertr_ != nullptr);

	hitTime_ = mngr_->addTimer();
	goBackTime_ =mngr_->addTimer();


	initialwidth_ = tr_->getWidth();
	initialheight_ = tr_->getHeight();
	initialpos_ = tr_->getPos();

	// Valores collider
	col_initialwidth_ = col_->getWidth();
	col_initialheight_ = col_->getHeight();

	maxWidth_ = 2 * initialwidth_;
	maxHeight_ = 2 * initialheight_;

}

void Punch::goDown()
{
	int objectivePos = sdlutils().height() - tr_->getHeight() - 100;
	

	if (tr_->getPos().getY() < objectivePos) {

		if(manoDerecha_) img_->setTexture(&sdlutils().images().at("manoDer_fondo"));
		else img_->setTexture(&sdlutils().images().at("manoIzq_fondo"));
		tr_->getVel().setY(handSpeed_);
		col_->setActive(false);
	}
	else {
		tr_->getPos().setY(objectivePos);
		changeState(FOLLOW);
	}
}

void Punch::followPlayer()
{
	auto handPos = tr_->getPos();
	auto playerPos = playertr_->getPos();

	if (!(handPos.getX() >= playerPos.getX() && handPos.getX() < playerPos.getX() + playertr_->getWidth())) {

		tr_->getVel() = (handPos.getX() > playerPos.getX() ? Vector2D(-handSpeed_, 0) : Vector2D(handSpeed_, 0));
	}
	else
	{
		hitTime_->reset();
		changeState(HIT);
	}
}

void Punch::hit()
{
	auto width = tr_->getWidth();
	auto height = tr_->getHeight();
	tr_->getVel().set(Vector2D(0, 0));

	if (manoDerecha_) img_->setTexture(&sdlutils().images().at("manoDer"));
	else img_->setTexture(&sdlutils().images().at("manoIzq"));

	if (hitTime_->currTime() >= 350) {
		col_->setWidth(width);
		col_->setHeight(height);

		changeState(BACK);
		goBackTime_->reset();

		col_->setIsTrigger(false);
		col_->setActive(false);
	}
	else {
		int nextWidth = std::min(width + dW, maxWidth_);
		int nextHeight = std::min(height + dH, maxHeight_);

		tr_->getPos().setY(tr_->getPos().getY() - (nextWidth - width));
		tr_->getPos().setX(tr_->getPos().getX() - (nextHeight - height));

		tr_->setWidth(nextWidth);
		tr_->setHeight(nextHeight);

		col_->setWidth(nextWidth);
		col_->setHeight(nextHeight);
		col_->setIsTrigger(true);
		col_->setActive(true);
	}
}

void Punch::goBack()
{
	auto width = tr_->getWidth();
	auto height = tr_->getHeight();

	if (goBackTime_->currTime() >= 750) {

		col_->setActive(false);

		bool isSize = false, isPos = false;

		if (manoDerecha_) img_->setTexture(&sdlutils().images().at("manoDer_fondo"));
		else img_->setTexture(&sdlutils().images().at("manoIzq_fondo"));

		if (abs(tr_->getPos().getX() - initialpos_.getX()) > handSpeed_ + 1 || abs(tr_->getPos().getY() - initialpos_.getY()) > handSpeed_ + 1) {
			Vector2D dir = initialpos_ - tr_->getPos();
			tr_->getVel().set(dir.normalize() * handSpeed_);
		}
		else { //correct pos
			tr_->getVel().set(Vector2D(0, 0));
			tr_->getPos().set(initialpos_);
			isPos = true;
		}


		if (abs(width - initialwidth_) > 1 || abs(height - initialheight_) > 1) {

			int nextWidth = std::max(width - dW, initialwidth_);
			int nextHeight = std::max(height - dH, initialheight_);

			tr_->getPos().setY(tr_->getPos().getY() + (nextWidth - width));
			tr_->getPos().setX(tr_->getPos().getX() + (nextHeight - height));

			tr_->setWidth(nextWidth);
			tr_->setHeight(nextHeight);
		}
		else { //correct size
			tr_->setWidth(initialwidth_);
			tr_->setHeight(initialheight_);
			isSize = true;
		}

		if (isPos && isSize) {
			changeState(FIN);

			if (manoDerecha_) img_->setTexture(&sdlutils().images().at("manoDer"));
			else img_->setTexture(&sdlutils().images().at("manoIzq"));

			col_->setWidth(col_initialwidth_);	
			col_->setHeight(col_initialheight_);
			col_->setActive(true);
			col_->setIsTrigger(false);
		}
	}

}