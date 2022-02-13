#include "SimplePhysicsPlayer.h"

#include "Container.h"

SimplePhysicsPlayer::SimplePhysicsPlayer(float gravity) : gravity_(gravity)
{
}

SimplePhysicsPlayer::~SimplePhysicsPlayer()
{
}

void SimplePhysicsPlayer::update(Container* o)
{
	o->getVel().set(Vector2D(o->getVel().getX(), o->getVel().getY() + gravity_));
}


