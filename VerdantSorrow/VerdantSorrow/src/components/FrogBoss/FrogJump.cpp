#include "FrogJump.h"
#include "../../sdlutils/SDLUtils.h"
#include "../Transform.h"
#include "../../ecs/Entity.h"

FrogJump::FrogJump(): tr_()
{
}

void FrogJump::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
}

void FrogJump::update()
{
}
