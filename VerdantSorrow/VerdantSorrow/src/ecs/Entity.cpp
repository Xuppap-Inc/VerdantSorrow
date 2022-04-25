// This file is part of the course TPV2@UCM - Samir Genaim

#include "Entity.h"
#include "Manager.h"

#include "../game/CollisionManager.h"

namespace ecs {

void ecs::Entity::addToGroupInTheManager(grpId_type gId) {
	mngr_->addToGroup(gId, this);
}

void Entity::deleteColliderFromManager()
{
	auto col = getComponent<RectangleCollider>();
	if (col != nullptr)
	{
		auto colMngr_ = mngr_->getColManager();
		colMngr_->deleteCollider(getComponent<RectangleCollider>());
	}
}

} // end of namespace
