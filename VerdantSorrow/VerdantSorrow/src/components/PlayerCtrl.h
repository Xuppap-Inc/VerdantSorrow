#pragma once
#include "../ecs/Component.h"

#include <SDL.h>
class Transform;
class PlayerCtrl : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_PLAYERCTRL)
	PlayerCtrl(float jumpForce, float speed) : tr_(nullptr), speed_(speed), jumpForce_(jumpForce), onGround_(false) {}
	virtual ~PlayerCtrl();
	void update() override;
	void initComponent() override;

	bool isOnGround();
	void setOnGround(bool set);
private:
	Transform* tr_;
	float jumpForce_;
	float speed_;

	bool onGround_;
};

