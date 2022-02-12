#pragma once
#include "Component.h"

class Container;

class RenderComponent : public Component {
public:
	RenderComponent(){}
	virtual ~RenderComponent(){}
	
	virtual void render(Container* o) = 0;
};