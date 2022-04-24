#pragma once
#include "../../../../ecs/Component.h"
#include "../../../../sdlutils/SDLUtils.h"
#include "../../../RectangleCollider.h"
#include "../../../../sdlutils/VirtualTimer.h"

class Transform;
class VirtualTimer;

class RootMovement : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_ROOTMOVEMENT)
	RootMovement();
	~RootMovement();

	void initComponent() override;
	void update() override;

private:

	enum State
	{
		APPEARING, GROWING, DYING
	};

	State state_;

	const int WAIT_TO_DIE = 700;

	const int MAX_HEIGH_BEFORE_GROWING = 100;

	Transform* tr_;
	RectangleCollider* col_;
	float speed_;
	VirtualTimer* waitTimer_;
};