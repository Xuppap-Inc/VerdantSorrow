#pragma once
#include "../../ecs/Component.h"
#include "PlayerAttributes.h"
#include "../RectangleCollider.h"
#include "../FramedImage.h"
#include <SDL.h>
#include <vector>
using namespace std;

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
	void animationManagement();
	void handleInput();
	bool isRolling() { return isRolling_; };
	void disableKnockback();

private:
	RectangleCollider* playerCol_;
	Transform* tr_;
	PlayerAttributes* attrib_;
	FramedImage* anim_;

	//variables de velocidad
	float jumpForce_, speed_, rollSpeed_, deceleration_;

	//variables de control de roll
	float rollCooldown_ , rollDuration_, lastRoll_;
	bool isRolling_;

	//bool de control de slide
	bool slide_;

	//variables del knockback
	float knockbackForceX_, knockbackForceY_;
	bool isKnockback;

	//Indica la direccion del movimiento del jugador
	int movementDir_;

	//booleanas de control para manejar el input
	bool moveLeft_, moveRight_, jump_, roll_;


	// Input
	vector<SDL_Scancode> jumpKeys;
	vector<SDL_GameControllerButton> jumpButtons;
	vector<SDL_Scancode> rollKeys;
	vector<SDL_GameControllerButton> rollButtons;
};

