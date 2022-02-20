#pragma once
#include "../../ecs/Component.h"
#include "PlayerAttributes.h"

#include <SDL.h>
class Transform;
class PlayerCtrl : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_PLAYERCTRL)
	PlayerCtrl(float jumpForce, float speed) : tr_(nullptr), speed_(speed), jumpForce_(jumpForce), attrib_(),movementDir_(1) {}
	virtual ~PlayerCtrl();
	void update() override;
	void initComponent() override;
	int getMovementDir() { return movementDir_; }

private:
	Transform* tr_;
	float jumpForce_;
	float speed_;

	//Indica la direccion del movimiento del jugador (se usa para la direccion de ataque y animacion?)
	int movementDir_;

	PlayerAttributes* attrib_;
};

