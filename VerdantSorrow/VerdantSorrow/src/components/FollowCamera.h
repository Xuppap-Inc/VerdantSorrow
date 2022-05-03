#pragma once

#include "../ecs/Component.h"

class Transform;
class FollowCamera : public ecs::Component
{

public:
	__CMPID_DECL__(ecs::_FOLLOW_CAMERA)
	FollowCamera();
	virtual ~FollowCamera();

	void initComponent() override;
	void update() override;

private:

	Transform* cameraTr_;
	Transform* tr_;
};

