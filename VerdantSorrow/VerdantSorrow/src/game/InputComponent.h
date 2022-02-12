#pragma once
#include "Component.h"

class Container;

class InputComponent : public Component {
public:
	InputComponent(){}
	virtual ~InputComponent() {}

	virtual void handleInput(Container* o) = 0;
};