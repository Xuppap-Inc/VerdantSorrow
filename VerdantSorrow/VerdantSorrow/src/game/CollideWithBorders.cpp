#include "CollideWithBorders.h"

#include "../sdlutils/SDLUtils.h"
#include "Container.h"

CollideWithBorders::CollideWithBorders(PlayerCtrl* controller): playerController_(controller)
{
}

CollideWithBorders::~CollideWithBorders()
{
}

void CollideWithBorders::update(Container* o)
{
	auto& pos = o->getPos();

	auto height = sdlutils().height();
	auto width = sdlutils().width();
	auto playerHeight = o->getHeight();
	auto playerWidth = o->getWidth();
	
	if (pos.getY() > height - playerHeight) {
	
		pos.set(Vector2D(pos.getX(), height - playerHeight));
		playerController_->setOnGround(true);
	}

	if (pos.getX() > width - playerWidth) {
	
		pos.set(Vector2D(width - playerWidth, pos.getY()));
	}
	else if (pos.getX() < 0) {

		pos.set(Vector2D(0, pos.getY()));
	}
}
