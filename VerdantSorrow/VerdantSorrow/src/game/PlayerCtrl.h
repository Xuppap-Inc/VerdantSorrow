#pragma once
#include "InputComponent.h"
#include <SDL.h>
#include "box2d.h"
class PlayerCtrl : public InputComponent
{
public:
	PlayerCtrl(float jumpForce, float speed, b2Body* body) : speed_(speed), jumpForce_(jumpForce),body_(body) {}
	virtual ~PlayerCtrl();
	void handleInput(Container* o);
private:
	float jumpForce_;
	float speed_;
	b2Body* body_;
};

