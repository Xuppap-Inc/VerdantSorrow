#pragma once
#include "../../ecs/Component.h"
#include "PlayerAttributes.h"
#include "../RectangleCollider.h"
#include <SDL.h>
class Transform;
class PlayerCtrl : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_PLAYERCTRL)
	PlayerCtrl(float jumpForce, float speed, float deceleration, float rollSpeed) : tr_(nullptr), speed_(speed), jumpForce_(jumpForce)
		, rollSpeed_(rollSpeed)	, deceleration_(deceleration), attrib_(),movementDir_(1) {}
	virtual ~PlayerCtrl();
	void update() override;
	void initComponent() override;
	int getMovementDir() { return movementDir_; }
	void changeSlideAtribute(bool value) { deslizar = value; }

private:
	RectangleCollider* playerCol_;
	Transform* tr_;
	float jumpForce_;
	float speed_;
	float rollSpeed_;
	float deceleration_ = 0.85;
	float rollCooldown = 1000, rollDuration = 500, lastRoll;
	bool deslizar = false, isRolling = false;

	//Indica la direccion del movimiento del jugador (se usa para la direccion de ataque y animacion?)
	int movementDir_;

	PlayerAttributes* attrib_;
};

