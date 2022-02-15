#include "SimplePhysicsPlayer.h"

#include "../ecs/Entity.h"
#include "Transform.h"

SimplePhysicsPlayer::SimplePhysicsPlayer(float gravity, CollisionManager* colManager) : tr_(nullptr), gravity_(gravity), colMan_(colManager), collider_(nullptr)
{
}

SimplePhysicsPlayer::~SimplePhysicsPlayer()
{
}

void SimplePhysicsPlayer::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	collider_ = ent_->getComponent<RectangleCollider>();
	assert(tr_ != nullptr && collider_ != nullptr);
}

void SimplePhysicsPlayer::update()
{
	tr_->getVel().set(Vector2D(tr_->getVel().getX(), tr_->getVel().getY() + gravity_));

	if (colMan_->hasCollisions(collider_)) {
	
		std::vector<RectangleCollider*> colliders = colMan_->getCollisions(collider_);

		for (auto c : colliders) {
		
			if (c->isActive() && !c->isTrigger()) {
			
				std::cout << "ha colisionado" << std::endl;
			}
		}
	}
}


