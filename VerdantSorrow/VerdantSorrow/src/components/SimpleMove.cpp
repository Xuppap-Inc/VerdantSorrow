#include "SimpleMove.h"

#include "../sdlutils/SDLUtils.h"
#include "../ecs/Entity.h"
#include "Transform.h"

SimpleMove::SimpleMove() {

}

SimpleMove::~SimpleMove() {
}

void SimpleMove::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
}

void SimpleMove::update() {

	auto& pos = tr_->getPos();
	auto& vel = tr_->getVel();

	pos = pos + vel;

}