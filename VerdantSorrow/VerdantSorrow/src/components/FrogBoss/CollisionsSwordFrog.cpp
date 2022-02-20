#include "CollisionsSwordFrog.h"
#include "../player/PlayerAttributes.h"
#include "../../ecs/Entity.h"
#include "../Transform.h"


CollisionsSwordFrog::CollisionsSwordFrog(CollisionManager* colManager) : colMan_(colManager), collider_(nullptr), invulnerable_(false), invTimer(0)
{
}

CollisionsSwordFrog::~CollisionsSwordFrog()
{
}

void CollisionsSwordFrog::initComponent()
{
	collider_ = ent_->getComponent<RectangleCollider>();
	attrib_ = ent_->getComponent<BossAtributos>();
	/*gravity_ = ent_->getComponent<SimpleGravity>();*/
	assert(collider_ != nullptr && attrib_ != nullptr);
}

void CollisionsSwordFrog::update()
{
	//Colisiones
	if (colMan_->hasCollisions(collider_)) {

		std::vector<RectangleCollider*> colliders = colMan_->getCollisions(collider_);

		for (auto c : colliders) {

			if (c->isActive() && !c->isTrigger()) {
				ecs::Entity* ent = c->getEntity();
				PlayerAttributes* playerAtributes = ent->getComponent<PlayerAttributes>();

				if (playerAtributes != nullptr) {
					if (!invulnerable_) {
						attrib_->setDamage(1);
						invulnerable_ = true;
						std::cout << attrib_->getLife() << std::endl;
						invTimer = sdlutils().currRealTime();
					}
				}
			}
		}

		if (invTimer + 5000 > sdlutils().currRealTime()) return;
		invulnerable_ = false;

	}
}


