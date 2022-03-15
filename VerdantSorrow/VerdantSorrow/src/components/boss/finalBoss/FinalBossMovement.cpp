#include "FinalBossMovement.h"
#include "../../../ecs/Entity.h"
#include "../../../ecs/Manager.h"
#include "../../Transform.h"
#include "../../../sdlutils/SDLUtils.h"

FinalBossMovement::FinalBossMovement():tr_(nullptr)
{
}

FinalBossMovement::~FinalBossMovement()
{
}

void FinalBossMovement::update()
{
	auto& pos_ = tr_->getPos();
	auto& vel_ = tr_->getVel();
	pos_ = pos_ + vel_;


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
	else if (pos_.getX() <0) {
		pos_.setX(0);
		vel_.setX(-vel_.getX());
	}
}

void FinalBossMovement::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
}
