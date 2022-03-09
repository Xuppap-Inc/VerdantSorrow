#include "RectangleCollider.h"
#include "../ecs/Entity.h"
#include "Transform.h"
#include "../sdlutils/SDLUtils.h"


RectangleCollider::RectangleCollider() : tr_(nullptr), height_(), width_(), pos_(), isTrigger_(false), active_(true)
{
}
RectangleCollider::RectangleCollider(float width, float height): tr_(nullptr), height_(height), width_(width), pos_(), isTrigger_(false), active_(true)
{
}

RectangleCollider::~RectangleCollider()
{
}

void RectangleCollider::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
}

void RectangleCollider::update()
{
	if (isActive())
		setPosition();
	
}


void RectangleCollider::debug()
{
	if (isActive())
		drawCollider();
}


SDL_Rect RectangleCollider::getCollider()
{
	return build_sdlrect(pos_, width_, height_);
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

void RectangleCollider::drawCollider()
{
	SDL_Rect r = getCollider();
	if(isTrigger())
		SDL_SetRenderDrawColor(sdlutils().renderer(), 0, 0, 255, 255);
	else	
		SDL_SetRenderDrawColor(sdlutils().renderer(), 255, 0, 0, 255);

	SDL_RenderDrawRect(sdlutils().renderer(),&r);
}

void RectangleCollider::setPosition()
{
	Vector2D contPos = tr_->getPos();
	pos_ = Vector2D(contPos.getX() + (tr_->getWidth() - width_) / 2, contPos.getY() + (tr_->getHeight() - height_));
}

Entity* RectangleCollider::getEntity() {
	return ent_;
}
