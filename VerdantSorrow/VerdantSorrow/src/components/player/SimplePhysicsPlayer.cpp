#include "SimplePhysicsPlayer.h"

#include "../../ecs/Entity.h"
#include "../Transform.h"

SimplePhysicsPlayer::SimplePhysicsPlayer(CollisionManager* colManager) : tr_(nullptr), colMan_(colManager), collider_(nullptr)
{
}

SimplePhysicsPlayer::~SimplePhysicsPlayer()
{
}

void SimplePhysicsPlayer::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	collider_ = ent_->getComponent<RectangleCollider>();
	attrib_ = ent_->getComponent<PlayerAttributes>();
	gravity_ = ent_->getComponent<SimpleGravity>();
	assert(tr_ != nullptr && collider_ != nullptr && attrib_ != nullptr);
}

void SimplePhysicsPlayer::update()
{
	//Gravedad
	if (!attrib_->isOnGround() && !gravity_->isActive()) gravity_->setActive(true);
	else if (gravity_->isActive()) gravity_->setActive(false);

	//Colisiones
	if (colMan_->hasCollisions(collider_)) {
	
		std::vector<RectangleCollider*> colliders = colMan_->getCollisions(collider_);

		for (auto c : colliders) {
		
			if (c->isActive() && !c->isTrigger()) {
			
				std::cout << "ha colisionado" << std::endl;
			}
		}
	}
}


