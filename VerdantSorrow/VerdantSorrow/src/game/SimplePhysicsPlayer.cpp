#include "SimplePhysicsPlayer.h"

#include "Container.h"

SimplePhysicsPlayer::SimplePhysicsPlayer(float gravity, CollisionManager* colManager, RectangleCollider* collider) : gravity_(gravity), colMan_(colManager), collider_(collider)
{
}

SimplePhysicsPlayer::~SimplePhysicsPlayer()
{
}

void SimplePhysicsPlayer::update(Container* o)
{
	o->getVel().set(Vector2D(o->getVel().getX(), o->getVel().getY() + gravity_));

	if (colMan_->hasCollisions(collider_)) {
	
		std::vector<RectangleCollider*> colliders = colMan_->getCollisions(collider_);

		for (auto c : colliders) {
		
			if (c->isActive() && !c->isTrigger()) {
			
				std::cout << "ha colisionado" << std::endl;
			}
		}
	}
}


