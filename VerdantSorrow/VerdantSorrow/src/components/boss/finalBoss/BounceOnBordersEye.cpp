#include "BounceOnBordersEye.h"

#include "../../../sdlutils/SDLUtils.h"
#include "../../Transform.h"
#include "../../../ecs/Entity.h"
#include "../BossAtributos.h"
#include "../../FramedImage.h"

BounceOnBordersEye::BounceOnBordersEye() : OnBorders()
{
	tr_ = nullptr;
}

BounceOnBordersEye::~BounceOnBordersEye()
{
}

void BounceOnBordersEye::initComponent()
{
	OnBorders::initComponent();
	attr_ = ent_->getComponent<BossAtributos>();
	sprite_ = ent_->getComponent<FramedImage>();
	assert(attr_ != nullptr &&sprite_!=nullptr);
}

void BounceOnBordersEye::onBottom()
{
	auto& pos = tr_->getPos();
	auto height = sdlutils().height();
	auto bossHeight = tr_->getHeight();

	pos.set(Vector2D(pos.getX(), height - bossHeight - GROUND_HEIGHT));
	
	auto& vel = tr_->getVel();
	vel.setY(-vel.getY());
}

void BounceOnBordersEye::onTop()
{
	auto& pos = tr_->getPos();

	pos.set(Vector2D(pos.getX(), 0));

	auto& vel = tr_->getVel();
	vel.setY(-vel.getY());
}

void BounceOnBordersEye::onLeft()
{
	auto& pos = tr_->getPos();
	attr_->setOnLeftBorder(true);

	pos.set(Vector2D(0, pos.getY()));
	
	auto& vel = tr_->getVel();
	vel.setX(-vel.getX());
}

void BounceOnBordersEye::onRight()
{
	auto& pos = tr_->getPos();

	auto width = sdlutils().width();
	auto bossWidth = tr_->getWidth();
	attr_->setOnRightBorder(true);

	pos.set(Vector2D(width - bossWidth, pos.getY()));
	
	auto& vel = tr_->getVel();
	vel.setX(-vel.getX());
}

void BounceOnBordersEye::onNoLeftAndRight()
{
	if (attr_->isOnRightBorder()) attr_->setOnRightBorder(false);
	if (attr_->isOnLeftBorder()) attr_->setOnLeftBorder(false);
}

void BounceOnBordersEye::onNoTopAndBot()
{
	if (attr_->isOnGround()) attr_->setOnGround(false);
}
	

