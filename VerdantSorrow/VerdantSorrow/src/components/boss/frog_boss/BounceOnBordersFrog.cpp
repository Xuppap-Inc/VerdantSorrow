#include "BounceOnBordersFrog.h"

#include "../../../sdlutils/SDLUtils.h"
#include "../../Transform.h"
#include "../../../ecs/Entity.h"
#include "../BossAtributos.h"
#include "../../FramedImage.h"

BounceOnBordersFrog::BounceOnBordersFrog() : OnBorders()
{
	tr_ = nullptr;
}

BounceOnBordersFrog::~BounceOnBordersFrog()
{
}

void BounceOnBordersFrog::initComponent()
{
	OnBorders::initComponent();
	attr_ = ent_->getComponent<BossAtributos>();
	sprite_ = ent_->getComponent<FramedImage>();
	assert(attr_ != nullptr &&sprite_!=nullptr);
}

void BounceOnBordersFrog::onBottom()
{
	auto& pos = tr_->getPos();
	auto& vel = tr_->getVel();
	vel.set(Vector2D(0,0));
	auto height = sdlutils().height();
	auto bossHeight = tr_->getHeight();	
	pos.set(Vector2D(pos.getX(), height - bossHeight - GROUND_HEIGHT));
	attr_->setOnGround(true);
}

void BounceOnBordersFrog::onLeft()
{
	auto& pos = tr_->getPos();

	pos.set(Vector2D(0, pos.getY()));
	auto& vel = tr_->getVel();
	vel.setX(-vel.getX());
	sprite_->flipX(true);
}

void BounceOnBordersFrog::onRight()
{
	auto& pos = tr_->getPos();

	auto width = sdlutils().width();
	auto bossWidth = tr_->getWidth();

	pos.set(Vector2D(width - bossWidth, pos.getY()));
	auto& vel = tr_->getVel();
	vel.setX(-vel.getX());
	sprite_->flipX(false);
}

void BounceOnBordersFrog::onNoTopAndBot()
{
	if (attr_->isOnGround()) attr_->setOnGround(false);
}
	

