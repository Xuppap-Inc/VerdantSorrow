#pragma once
#include "../../../ecs/Component.h"
#include "../../../sdlutils/SDLUtils.h"

class Transform;
class FramedImage;
class FrogJump : public ecs::Component 
{
public:
	__CMPID_DECL__(ecs::_FROGJUMP)
	FrogJump();
	FrogJump(float jumpForce);

	void initComponent() override;
	void attack(int dir);

private:
	Transform* tr_;
	FramedImage* fr_;

	bool isJumping_;
	float jumpForce_;
	
	Uint32 lastJump_;
	Uint32 jumpCd_;
};