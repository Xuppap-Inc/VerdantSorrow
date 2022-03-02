#pragma once
#include "../../ecs/Component.h"
#include "root/RootMovement.h"

class RootWave : public ecs::Component
{
public:
	RootWave();
	~RootWave();

	void initComponent() override;
	void attack(int dir);

private:
	Transform* tr_;

	Uint32 lastTime;
};

