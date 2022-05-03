#include "ChangeVertical.h"

#include "../../../ecs/Entity.h"
#include "../../Transform.h"
#include "../wave/WaveMovement.h"
#include "../frog_boss/TongueAttack.h"
#include "../../player/PlayerAttributes.h"
#include "../../../ecs/Manager.h"
#include "../../ScrollCamera.h"

ChangeVertical::ChangeVertical(CollisionManager* colManager) : tr_(nullptr), colMan_(colManager), collider_(nullptr)
{
}

ChangeVertical::~ChangeVertical()
{
}

void ChangeVertical::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	collider_ = ent_->getComponent<RectangleCollider>();

	assert(tr_ != nullptr && collider_ != nullptr);
	collider_->setIsTrigger(true);
}

void ChangeVertical::update()
{
	//Colisiones
	if (colMan_->hasCollisions(collider_)) {

		for (auto c : colMan_->getCollisions(collider_)) {
			if (c->isActive() && !c->isTrigger() && c->getEntity()->getComponent<PlayerAttributes>()) {
				auto cameraC=mngr_->getHandler(ecs::_hdlr_CAMERA)->getComponent<ScrollCamera>();
				cameraC->lock(true, false);
				//colisiones
				

			}
		}
	}
}


