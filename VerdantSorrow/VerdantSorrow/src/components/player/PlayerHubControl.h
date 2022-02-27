#pragma once
#include "../../ecs/Component.h"
#include "PlayerAttributes.h"
#include "../RectangleCollider.h"
#include <SDL.h>
class Transform;
class PlayerHubControl: public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_PLAYERCTRL)
		PlayerHubControl(float jumpForce, float speed, float deceleration, float rollSpeed) : tr_(nullptr),
		 deceleration_(deceleration), attrib_() {}
	virtual ~PlayerHubControl();
	void update() override;
	void initComponent() override;

private:
	RectangleCollider* playerCol_;
	Transform* tr_;
	float deceleration_ = 0.85;
	float rollCooldown = 1000, rollDuration = 500, lastRoll;
	bool deslizar = false, isRolling = false;


	PlayerAttributes* attrib_;
};
