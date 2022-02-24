#pragma once
#include "../../ecs/Component.h"
#include "../../sdlutils/SDLUtils.h"

class Transform;

class FrogBigJumpLateral : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_FROGBIGJUMPLATERAL)
		FrogBigJumpLateral();
	FrogBigJumpLateral(float jumpForce);

	void initComponent() override;
	void update() override;

private:
	Transform* tr_;
	bool isJumping_;
	float jumpForce_;
	Uint32 lastJump_;
	Uint32 jumpCd_;
};