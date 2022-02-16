#pragma once
#include "../../ecs/Component.h"

class Transform;

class FrogJump : public ecs::Component 
{
public:
	__CMPID_DECL__(ecs::_FROGJUMP)
	FrogJump();

	void initComponent() override;
	void update() override;

private:
	Transform* tr_;
};