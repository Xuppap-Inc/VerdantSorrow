#include "PlayerAttributes.h"
#include "../../ecs/Entity.h"

PlayerAttributes::PlayerAttributes(): lives_(20), invulnerableTimer_(0), onGround(false), rightStop(false), leftStop(false), invulnerable_(false)
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

bool PlayerAttributes::isDownStop()
{
	return downStop;
}

void PlayerAttributes::setDownStop(bool set)
{
	downStop = set;
}

void PlayerAttributes::damagePlayer(int lives) {
	lives_ -= lives;
}

int PlayerAttributes::getLives()
{
	return lives_;
}
