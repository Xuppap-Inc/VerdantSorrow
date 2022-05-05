#pragma once
#include "../../../ecs/Component.h"

class Transform;
class VirtualTimer;
class FlyMovement :public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_FLY_MOVEMENT)
	FlyMovement();
	~FlyMovement();
	void initComponent() override;
	void update() override;
private:

	const float SPEED = 2.0;

	Transform* tr_;
	Transform* playerTr_;
	int dirX_, dirY_, timeOfLife_;
	int spaceWhereExplotes;
	VirtualTimer* vt_;
};

