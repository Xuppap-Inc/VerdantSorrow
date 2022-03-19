#include "LanternCollisions.h"
#include "../../../ecs/Entity.h"
#include "../../../ecs/Manager.h"
#include "../../Transform.h"
#include "../BossAtributos.h"
#include "../../RectangleCollider.h"
#include "LanternMovement.h"
#include "root/RootMovement.h"
#include "../../player/Attack.h"

LanternCollisions::LanternCollisions(CollisionManager* colManager) : attrib_(), lanternTr_(), collider_(), colMan_(colManager), damaged_(false), 
lanterMov_(), secondPhase_(false), contAttacks_(0)
{
}

LanternCollisions::~LanternCollisions()
{
}

void LanternCollisions::initComponent()
{
	lanternTr_ = ent_->getComponent<Transform>();
	lanterMov_ = ent_->getComponent<LanternMovement>();

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
				
				Attack* attack = ent->getComponent<Attack>();

				if (!damaged_ && rootMov != nullptr) {
					std::cout << "daño" << std::endl;
					attrib_->setDamage(3);
					damaged_ = true;
				}

				else if (secondPhase_ && attack != nullptr) {
				
					contAttacks_++;

					if (contAttacks_ >= NUM_ATTACKS_TO_REPOSITION) {
					
						contAttacks_ = 0;

						lanterMov_->moveToSide();
					}
				}
			}
		}
	}
}

void LanternCollisions::setDamaged(bool set)
{
	damaged_ = set;
}

void LanternCollisions::changeToSecondPhase()
{
	secondPhase_ = true;
}
