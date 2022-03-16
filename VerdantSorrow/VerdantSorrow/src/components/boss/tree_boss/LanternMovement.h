#pragma once
#include "TreeMovement.h"
#include "../../../ecs/Component.h"
#include "../../../sdlutils/SDLUtils.h"

class Transform;
class LanternMovement: public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_LANTERNMOVEMENT)

		LanternMovement();
	LanternMovement(Transform* treeTr_);
	~LanternMovement();
	void initComponent()override;
	virtual void update()override;

private:
	Transform* lanternTr_;
	Transform* treeTr_;
	
};

