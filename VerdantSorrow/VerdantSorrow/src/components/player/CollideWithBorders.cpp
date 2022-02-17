#include "CollideWithBorders.h"

#include "../../sdlutils/SDLUtils.h"
#include "../../ecs/Entity.h"
#include "../Transform.h"

CollideWithBorders::CollideWithBorders(): tr_(nullptr), attrib_(nullptr)
{
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

void CollideWithBorders::update()
{
	
	auto& pos = tr_->getPos();
	auto& vel = tr_->getVel();

	auto height = sdlutils().height();
	auto width = sdlutils().width();
	auto playerHeight = tr_->getHeight();
	auto playerWidth = tr_->getWidth();
	
	//Borde inferior
	if (pos.getY() > height - playerHeight) {
	
		if (!attrib_->isOnGround()) {
			
			attrib_->setOnGround(true);
			pos.set(Vector2D(pos.getX(), height - playerHeight));
			vel.set(Vector2D(vel.getX(), 0));
		}
	}

	//Borde derecho
	if (pos.getX() > width - playerWidth) {
	
		pos.set(Vector2D(width - playerWidth, pos.getY()));
		attrib_->setRightStop(true);
	}
	//Borde izquierdo
	else if (pos.getX() < 0) {

		pos.set(Vector2D(0, pos.getY()));
		attrib_->setLeftStop(true);
	}

	//si no toca bordes laterales pone las variables a false
	else {
		
		if (attrib_->isRightStop()) attrib_->setRightStop(false);
		if (attrib_->isLeftStop()) attrib_->setLeftStop(false);
	}
}
