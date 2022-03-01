#pragma once
#include "../../ecs/Component.h"
#include "../../sdlutils/SDLUtils.h"

class Transform;

class RootMovement : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_ROOTMOVEMENT)
		RootMovement();
	RootMovement(int dir, float speed);

	void initComponent() override;
	void update() override;

private:
	Transform* tr_;
};