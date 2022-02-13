#include "RectangleCollider.h"
#include "Container.h"

RectangleCollider::RectangleCollider(float width, float height): height_(height), width_(width), pos_(), isTrigger_(false), active_(true)
{
}

RectangleCollider::~RectangleCollider()
{
}

void RectangleCollider::update(Container* o)
{
	Vector2D contPos;

	pos_ = Vector2D(contPos.getX(), contPos.getY());
}

SDL_Rect RectangleCollider::getCollider()
{
	return {(int)pos_.getX(), (int)pos_.getY(), (int)width_, (int)height_};
}

void RectangleCollider::setIsTrigger(bool set)
{
	isTrigger_ = set;
}

bool RectangleCollider::isTrigger()
{
	return isTrigger_;
}

void RectangleCollider::setActive(bool set)
{
	active_ = set;
}

bool RectangleCollider::isActive()
{
	return active_;
}
