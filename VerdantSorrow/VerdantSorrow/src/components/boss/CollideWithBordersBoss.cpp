#include "CollideWithBordersBoss.h"

#include "../../sdlutils/SDLUtils.h"
#include "../Transform.h"
#include "../../ecs/Entity.h"
#include "BossAtributos.h"
#include "../FramedImage.h"

CollideWithBordersBoss::CollideWithBordersBoss() : OnBorders()
{
	tr_ = nullptr;
}

CollideWithBordersBoss::~CollideWithBordersBoss()
{
}

void CollideWithBordersBoss::initComponent()
{
	OnBorders::initComponent();
	attr_ = ent_->getComponent<BossAtributos>();
	sprite_ = ent_->getComponent<FramedImage>();
	assert(attr_ != nullptr &&sprite_!=nullptr);
}

void CollideWithBordersBoss::onBottom()
{
	auto& pos = tr_->getPos();
	auto& vel = tr_->getVel();
	vel.set(Vector2D(0,0));
	auto height = sdlutils().height();
	auto bossHeight = tr_->getHeight();	
	pos.set(Vector2D(pos.getX(), height - bossHeight - floorOffset_));
	attr_->setOnGround(true);
}

void CollideWithBordersBoss::onLeft()
{
	auto& pos = tr_->getPos();
	attr_->setOnLeftBorder(true);
	pos.set(Vector2D(0, pos.getY()));
	auto& vel = tr_->getVel();
	vel.setX(-vel.getX());
	sprite_->flipX(true);
}

void CollideWithBordersBoss::onRight()
{
	auto& pos = tr_->getPos();

	auto width = sdlutils().width();
	auto bossWidth = tr_->getWidth();
	attr_->setOnRightBorder(true);
	pos.set(Vector2D(width - bossWidth, pos.getY()));
	auto& vel = tr_->getVel();
	vel.setX(-vel.getX());
	sprite_->flipX(false);
}

void CollideWithBordersBoss::onNoLeftAndRight()
{
	if (attr_->isOnRightBorder()) attr_->setOnRightBorder(false);
	if (attr_->isOnLeftBorder()) attr_->setOnLeftBorder(false);
}

void CollideWithBordersBoss::onNoTopAndBot()
{
	if (attr_->isOnGround()) attr_->setOnGround(false);
}
	

