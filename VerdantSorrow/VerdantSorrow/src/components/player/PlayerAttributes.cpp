#include "PlayerAttributes.h"
#include "../../ecs/Entity.h"

#include <iostream>
PlayerAttributes::PlayerAttributes(): lives_(3), onGround(false), rightStop(false), leftStop(false)
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
	//std::cout << "Player ground: " << onGround << std::endl;

}

bool PlayerAttributes::isRightStop()
{
	return rightStop;
}

void PlayerAttributes::setRightStop(bool set)
{
	rightStop = set;
}

bool PlayerAttributes::isLeftStop()
{
	return leftStop;
}

void PlayerAttributes::setLeftStop(bool set)
{
	leftStop = set;
}

void PlayerAttributes::damagePlayer(int lives) {
	lives_ -= lives;
}

int PlayerAttributes::getLives()
{
	return lives_;
}
