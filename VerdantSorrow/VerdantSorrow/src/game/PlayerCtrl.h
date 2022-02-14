#pragma once
#include "InputComponent.h"
#include <SDL.h>
class PlayerCtrl : public InputComponent
{
public:
	PlayerCtrl(float jumpForce, float speed) : speed_(speed), jumpForce_(jumpForce) {}
	virtual ~PlayerCtrl();
	void handleInput(Container* o);
private:
	float jumpForce_;
	float speed_;
};

