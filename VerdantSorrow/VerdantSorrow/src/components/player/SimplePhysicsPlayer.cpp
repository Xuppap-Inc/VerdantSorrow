#include "SimplePhysicsPlayer.h"
#include "../BossAtributos.h"
#include "../../ecs/Entity.h"
#include "../Transform.h"


SimplePhysicsPlayer::SimplePhysicsPlayer(CollisionManager* colManager) : tr_(nullptr), colMan_(colManager), collider_(nullptr), invulnerable_(false), invTimer(0)
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
			
				
				ecs::Entity* ent = c->getEntity();
				BossAtributos* bA = ent->getComponent<BossAtributos>();
				
				if (bA != nullptr) {
					
					if (!invulnerable_) {
						attrib_->damagePlayer(1);
						invulnerable_ = true;
						std::cout << "a" << std::endl;
						invTimer = sdlutils().currRealTime();
					}
										
				}
				/*std::cout << "ha colisionado" << std::endl;*/
				
			}			
		}

		if (invTimer + 5000 > sdlutils().currRealTime()) return;
		invulnerable_ = false;
		
	}
}


