#include "CollideWithBordersBoss.h"

#include "../../sdlutils/SDLUtils.h"
#include "../../ecs/Entity.h"
#include "../Transform.h"

CollideWithBordersBoss::CollideWithBordersBoss() : tr_(nullptr)
{
}

CollideWithBordersBoss::~CollideWithBordersBoss()
{
}

void CollideWithBordersBoss::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);

}

void CollideWithBordersBoss::update()
{

	auto& pos = tr_->getPos();

	auto height = sdlutils().height();
	auto width = sdlutils().width();
	auto bossHeight = tr_->getHeight();
	auto bossWidth = tr_->getWidth();

	if (pos.getY() > height - bossHeight) {

		pos.set(Vector2D(pos.getX(), height - bossHeight));
	}

	if (pos.getX() > width - bossWidth) {

		pos.set(Vector2D(width - bossWidth, pos.getY()));
	}
	else if (pos.getX() < 0) {

		pos.set(Vector2D(0, pos.getY()));
	}
}
