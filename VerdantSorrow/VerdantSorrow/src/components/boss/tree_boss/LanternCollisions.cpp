#include "LanternCollisions.h"
#include "../../../ecs/Entity.h"
#include "../../../ecs/Manager.h"
#include "../../Transform.h"
#include "../BossAtributos.h"
#include "../../RectangleCollider.h"
#include "LanternMovement.h"
#include "root/RootMovement.h"
#include "../../player/Attack.h"
#include "RootAutoAim.h"

LanternCollisions::LanternCollisions(CollisionManager* colManager) : attrib_(), lanternTr_(), collider_(), colMan_(colManager), damaged_(false), 
lanterMov_(), secondPhase_(false), contAttacks_(0), attacked_(false), playerAttack_()
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

	playerAttack_ = mngr_->getHandler(ecs::_PLAYER)->getComponent<Attack>();

	bool correct = lanternTr_ != nullptr && lanterMov_ != nullptr && attrib_ != nullptr && collider_ != nullptr && playerAttack_ != nullptr;

	assert(correct);
}

void LanternCollisions::update()
{
	if (colMan_->hasCollisions(collider_)) {

		for (auto c : colMan_->getCollisions(collider_)) {

			if (c->isActive() && c->isTrigger()) {

				ecs::Entity* ent = c->getEntity();
				RootMovement* rootMov = ent->getComponent<RootMovement>();
				
				Attack* attack = ent->getComponent<Attack>();

				if (!damaged_ && rootMov != nullptr) {
					attrib_->setDamage(ROOT_DAMAGE_TO_LAMP);
					damaged_ = true;

					mngr_->getHandler(ecs::_TREEBOSS)->getComponent<RootAutoAim>()->cancelAttack();
				}

				else if (secondPhase_ && !attacked_ && attack != nullptr) {
				
					contAttacks_++;

					attacked_ = true;

					if (contAttacks_ >= NUM_ATTACKS_TO_REPOSITION) {
					
						contAttacks_ = 0;

						lanterMov_->moveToSide();
					}
				}
			}
		}
	}

	if (attacked_ && playerAttack_->hasFinished()) attacked_ = false;
}

void LanternCollisions::setDamaged(bool set)
{
	damaged_ = set;
}

void LanternCollisions::changeToSecondPhase()
{
	secondPhase_ = true;
}
