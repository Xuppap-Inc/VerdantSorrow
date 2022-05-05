#include "LightFollows.h"

#include "../../ecs/Manager.h"
#include "../../ecs/Entity.h"
#include "../../sdlutils/SDLUtils.h"	
#include "../Transform.h"


LightFollows::LightFollows(Transform* follows) : tr_(), follows_(follows)
{

}
LightFollows::~LightFollows() {
}

void LightFollows::initComponent() {
	tr_ = ent_->getComponent<Transform>();
}

void LightFollows::update() {

		tr_->getPos().setX((follows_->getPos().getX() + follows_->getWidth() / 2) - (tr_->getWidth() / 2));
		tr_->getPos().setY((follows_->getPos().getY() + follows_->getHeight() / 2) - (tr_->getHeight() / 2));
}
