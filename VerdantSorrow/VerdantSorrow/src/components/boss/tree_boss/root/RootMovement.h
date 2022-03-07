#pragma once
#include "../../../../ecs/Component.h"
#include "../../../../sdlutils/SDLUtils.h"
#include "../../../RectangleCollider.h"

class Transform;

class RootMovement : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_ROOTMOVEMENT)
	RootMovement();
	~RootMovement();

	void initComponent() override;
	void update() override;

private:
	Transform* tr_;
	RectangleCollider* col_;
	float speed_;
	float lastTime_;
};