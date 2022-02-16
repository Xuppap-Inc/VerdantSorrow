#include "CollideWithBorders.h"

#include "../sdlutils/SDLUtils.h"
#include "../ecs/Entity.h"
#include "Transform.h"

CollideWithBorders::CollideWithBorders(): tr_(nullptr), attrib_(nullptr)
{
}

CollideWithBorders::~CollideWithBorders()
{
}

void CollideWithBorders::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);

	attrib_ = ent_->getComponent<PlayerAttributes>();
}

void CollideWithBorders::update()
{
	
	auto& pos = tr_->getPos();

	auto height = sdlutils().height();
	auto width = sdlutils().width();
	auto playerHeight = tr_->getHeight();
	auto playerWidth = tr_->getWidth();
	
	if (pos.getY() > height - playerHeight) {
	
		pos.set(Vector2D(pos.getX(), height - playerHeight));
		if (!attrib_->isOnGround()) attrib_->setOnGround(true);
	}

	if (pos.getX() > width - playerWidth) {
	
		pos.set(Vector2D(width - playerWidth, pos.getY()));
	}
	else if (pos.getX() < 0) {

		pos.set(Vector2D(0, pos.getY()));
	}
}
