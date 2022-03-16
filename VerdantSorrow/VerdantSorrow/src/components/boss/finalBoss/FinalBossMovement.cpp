#include "FinalBossMovement.h"
#include "../../../ecs/Entity.h"
#include "../../../ecs/Manager.h"
#include "../../Transform.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../boss/BossAtributos.h"
#include "HandsManager.h"

FinalBossMovement::FinalBossMovement() :tr_(nullptr), bA_(nullptr), handMngr_(nullptr), phase_(PHASE1), eyeState_(BOUNCE), eyeSpeed(4)
{
}

FinalBossMovement::~FinalBossMovement()
{
}

void FinalBossMovement::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	bA_ = ent_->getComponent<BossAtributos>();
	handMngr_ = ent_->getComponent<HandsManager>();
	assert(tr_ != nullptr, bA_ != nullptr, handMngr_ != nullptr);
}

void FinalBossMovement::update()
{
	if (phase_ == PHASE1) {
		if (bA_->getLife() <= bA_->getMaxHp() / 2)
			phase_ = PHASE2;

		tr_->getVel() = Vector2D(0, 0);//ESTO SE VA
	}
	else if (phase_ == PHASE2) {
		bounce();
	}
}

void FinalBossMovement::bounce()
{
	auto& pos_ = tr_->getPos();
	auto& vel_ = tr_->getVel();

	if (vel_.magnitude() == 0)
		vel_ = Vector2D(1,1);


	// bounce on top/bottom borders
	if (pos_.getY() < 0) {
		pos_.setY(0.0f);
		vel_.setY(-vel_.getY());
		//sdlutils().soundEffects().at("wall_hit").play();
	}
	else if (pos_.getY() + tr_->getHeight() > sdlutils().height()) {
		pos_.setY(sdlutils().height() - tr_->getHeight());
		vel_.setY(-vel_.getY());
		//sdlutils().soundEffects().at("wall_hit").play();
	}
	else if (pos_.getX() + tr_->getWidth() > sdlutils().width()) {
		pos_.setX(sdlutils().width() - tr_->getHeight());
		vel_.setX(-vel_.getX());
		//sdlutils().soundEffects().at("wall_hit").play();
	}
	else if (pos_.getX() < 0) {
		pos_.setX(0);
		vel_.setX(-vel_.getX());
	}

	vel_ = vel_.normalize() * eyeSpeed;
}

