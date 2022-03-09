#include "ClapAttack.h"
#include "../../../ecs/Entity.h"
#include "../../Transform.h"
#include "../../RectangleCollider.h"
#include "../../../sdlutils/SDLUtils.h"

ClapAttack::ClapAttack(Entity* leftHand, Entity* rightHand): leftHand_(leftHand), rightHand_(rightHand)
{
}

ClapAttack::~ClapAttack()
{
}

void ClapAttack::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr, collider_ != nullptr, treeMovement_ != nullptr);
}

void ClapAttack::update()
{

	
}
