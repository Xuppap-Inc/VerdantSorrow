#pragma once
#include "../../ecs/Component.h"
#include "../../sdlutils/SDLUtils.h"

class Transform;

class WaveMovement : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_WAVEMOVEMENT)
		WaveMovement();
	WaveMovement(int speed, float jumpForce);

	void initComponent() override;
	void update() override;

private:
	Transform* tr_;
	int dir_;
	float speed_;
};