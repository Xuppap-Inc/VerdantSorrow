#include "PlayerAttributes.h"
#include "../../ecs/Entity.h"

PlayerAttributes::PlayerAttributes(): lives(3), onGround(false)
{
}

PlayerAttributes::~PlayerAttributes()
{
}

bool PlayerAttributes::isOnGround()
{
	return onGround;
}

void PlayerAttributes::setOnGround(bool set)
{
	onGround = set;
}
