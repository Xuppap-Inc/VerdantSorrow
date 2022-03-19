#pragma once
#include "../../../ecs/Component.h"
#include "../../../sdlutils/SDLUtils.h"

class Transform;
class LanternMovement: public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_LANTERNMOVEMENT)

	LanternMovement();
	~LanternMovement();
	void initComponent()override;
	virtual void update()override;

	bool isActive();
	void setActive(bool set);

	void moveToSide();

private:
	Transform* lanternTr_;
	Transform* treeTr_;
	
	bool active;

	bool rightSide;
};

