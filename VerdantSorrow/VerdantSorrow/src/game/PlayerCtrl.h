#pragma once
#include "InputComponent.h"
#include <SDL.h>
class PlayerCtrl : public InputComponent
{
public:
	PlayerCtrl(float jumpForce, float speed) : speed_(speed), jumpForce_(jumpForce), onGround_(false) {}
	virtual ~PlayerCtrl();
	void handleInput(Container* o);

	bool isOnGround();
	void setOnGround(bool set);
private:
	float jumpForce_;
	float speed_;

	bool onGround_;
};

