#include "../../ecs/Component.h"


#pragma once
class TutorialFly :public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_TUTORIAL_FLY)
	TutorialFly();
	~TutorialFly();
	void receiveHit();
};


