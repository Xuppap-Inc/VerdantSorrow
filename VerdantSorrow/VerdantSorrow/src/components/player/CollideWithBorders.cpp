#include "CollideWithBorders.h"

#include "../../sdlutils/SDLUtils.h"
#include "../../ecs/Entity.h"
#include "../Transform.h"

CollideWithBorders::CollideWithBorders(): OnBorders()
{
	attrib_ = nullptr;
	tr_ = nullptr;
}

CollideWithBorders::~CollideWithBorders()
{
}

void CollideWithBorders::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	attrib_ = ent_->getComponent<PlayerAttributes>();
	assert(tr_ != nullptr && attrib_ != nullptr);
}

void CollideWithBorders::onBottom()
{
	auto& pos = tr_->getPos();
	auto& vel = tr_->getVel();

	auto height = sdlutils().height();
	auto playerHeight = tr_->getHeight();

	if (!attrib_->isOnGround()) {

		attrib_->setOnGround(true);
		pos.set(Vector2D(pos.getX(), height - playerHeight));
		vel.set(Vector2D(vel.getX(), 0));
	}
}

void CollideWithBorders::onLeft()
{
	auto& pos = tr_->getPos();

	pos.set(Vector2D(0, pos.getY()));
	attrib_->setLeftStop(true);
}

void CollideWithBorders::onRight()
{
	auto& pos = tr_->getPos();

	auto width = sdlutils().width();
	auto playerWidth = tr_->getWidth();

	pos.set(Vector2D(width - playerWidth, pos.getY()));
	attrib_->setRightStop(true);
}

void CollideWithBorders::onNoLeftAndRight()
{
	if (attrib_->isRightStop()) attrib_->setRightStop(false);
	if (attrib_->isLeftStop()) attrib_->setLeftStop(false);
}