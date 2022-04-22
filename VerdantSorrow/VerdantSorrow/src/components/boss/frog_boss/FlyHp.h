#pragma once
#include "../../../ecs/Component.h"

class FrogAttackManager;
class FlyHp:public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_FLY_HP)
	FlyHp();
	FlyHp(FrogAttackManager* fAM);
	~FlyHp();
	void receiveHit();
protected:
	FrogAttackManager* fAM_;
};

