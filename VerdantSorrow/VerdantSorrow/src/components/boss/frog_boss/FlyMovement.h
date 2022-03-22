#pragma once
#include "../../../ecs/Component.h"

class Transform;
class FlyMovement :public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_FLY_MOVEMENT)
	FlyMovement();
	FlyMovement(int dir);
	~FlyMovement();
	void initComponent() override;
	void update() override;
private:
	Transform* tr_;
	int dir_;
};

