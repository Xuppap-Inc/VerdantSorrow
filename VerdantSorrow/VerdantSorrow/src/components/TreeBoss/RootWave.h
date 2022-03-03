#pragma once
#include "../../ecs/Component.h"
#include "root/RootMovement.h"
#include "TreeAttackManager.h"

class RootWave : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_ROOTWAVE)

	RootWave(CollisionManager* collManager);
	~RootWave();

	void initComponent() override;
	void update() override;
	void attack(int dir);
	void createRoot(int x);

private:
	Transform* tr_;
	CollisionManager* collManager_;

	Uint32 lastTime;
};

