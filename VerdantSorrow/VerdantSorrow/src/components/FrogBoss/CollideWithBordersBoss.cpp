#include "CollideWithBordersBoss.h"

#include "../../sdlutils/SDLUtils.h"
#include "../Transform.h"

CollideWithBordersBoss::CollideWithBordersBoss() : OnBorders()
{
	tr_ = nullptr;
}

CollideWithBordersBoss::~CollideWithBordersBoss()
{
}

void CollideWithBordersBoss::onBottom()
{
	auto& pos = tr_->getPos();
	auto& vel = tr_->getVel();

	auto height = sdlutils().height();
	auto bossHeight = tr_->getHeight();
	vel.setX(0);
	pos.set(Vector2D(pos.getX(), height - bossHeight));
}

void CollideWithBordersBoss::onLeft()
{
	auto& pos = tr_->getPos();

	pos.set(Vector2D(0, pos.getY()));
}

void CollideWithBordersBoss::onRight()
{
	auto& pos = tr_->getPos();

	auto width = sdlutils().width();
	auto bossWidth = tr_->getWidth();

	pos.set(Vector2D(width - bossWidth, pos.getY()));
}
