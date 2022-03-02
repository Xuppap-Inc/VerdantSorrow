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
	PlayerCtrl(float jumpForce, float speed, float deceleration, float rollSpeed);
	~PlayerCtrl();
	void update() override;
	void initComponent() override;

	int getMovementDir() { return movementDir_; }

	void doKnockback(int dir);
	void doAttack();
	void doSlide();
	void handleInput();

private:
	RectangleCollider* playerCol_;
	Transform* tr_;
	PlayerAttributes* attrib_;

	//variables de velocidad
	float jumpForce_, speed_, rollSpeed_, deceleration_;

	//variables de control de roll
	float rollCooldown_ , rollDuration_, lastRoll_;
	bool isRolling_;

	//bool de control de slide
	bool slide_;

	//variables del knockback
	float knockbackForceX_, knockbackForceY_;

	//Indica la direccion del movimiento del jugador
	int movementDir_;

	//booleanas de control para manejar el input
	bool moveLeft_, moveRight_, jump_, roll_;

};

