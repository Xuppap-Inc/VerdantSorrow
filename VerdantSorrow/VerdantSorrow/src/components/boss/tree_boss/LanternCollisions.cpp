#include "LanternCollisions.h"
#include "../../../ecs/Entity.h"
#include "../../../ecs/Manager.h"
#include "../../../components/Transform.h"
#include "../../../components/boss/BossAtributos.h"
#include "../../../components/RectangleCollider.h"
#include "root/RootMovement.h"

LanternCollisions::LanternCollisions(CollisionManager* colManager) : attrib_(), lanternTr_(), collider_(), colMan_(colManager), damaged_(false)
{
}

LanternCollisions::~LanternCollisions()
{
}

void LanternCollisions::initComponent()
{
	lanternTr_ = ent_->getComponent<Transform>();
	attrib_ = ent_->getComponent<BossAtributos>();
	collider_ = ent_->getComponent<RectangleCollider>();
	assert(lanternTr_ != nullptr);
}

void LanternCollisions::update()
{
	if (colMan_->hasCollisions(collider_)) {

		std::vector<RectangleCollider*> colliders = colMan_->getCollisions(collider_);

		for (auto c : colliders) {

			if (c->isActive() && c->isTrigger()) {

				ecs::Entity* ent = c->getEntity();
				RootMovement* rootMov = ent->getComponent<RootMovement>();

				if (damaged_ && rootMov != nullptr) {

					attrib_->setDamage(1);
					damaged_ = true;
				}
			}
		}
	}
}

void LanternCollisions::setDamaged(bool set)
{
	damaged_ = set;
}
