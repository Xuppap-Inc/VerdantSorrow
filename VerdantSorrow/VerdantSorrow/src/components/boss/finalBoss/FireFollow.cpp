#include "FireFollow.h"
#include "../../../sdlutils/SDLUtils.h"
#include "HandsManager.h"
#include "../../../ecs/Entity.h"
#include "../../../ecs/Manager.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../Transform.h"
#include "../../RectangleCollider.h"



FireFollow::FireFollow(ecs::Entity* hand) : tr_(), hand_(hand), handTr_()
{
}

FireFollow::~FireFollow()
{
}

void FireFollow::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
	handTr_ = hand_->getComponent<Transform>();
	assert(handTr_ != nullptr);
}

void FireFollow::update() {
	tr_->getPos().set(handTr_->getPos().getX(), handTr_->getPos().getY());
}

