#include "FollowCamera.h"

#include "../ecs/Manager.h"
#include "Transform.h"

FollowCamera::FollowCamera() : cameraTr_(), tr_()
{
}

FollowCamera::~FollowCamera()
{
}

void FollowCamera::initComponent()
{
	cameraTr_ = mngr_->getHandler(ecs::_hdlr_CAMERA)->getComponent<Transform>();
	tr_ = ent_->getComponent<Transform>();

	assert(cameraTr_ != nullptr && tr_ != nullptr);
}

void FollowCamera::update()
{
	auto cameraPos = cameraTr_->getPos();

	tr_->getPos().set(cameraPos.getX(), cameraPos.getY());
}
