#pragma once
#include "../../ecs/Component.h"
#include "RootSpawner.h"

class RootWave : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_ROOTSPAWNER)

	RootWave();
	~RootWave();

	void initComponent() override;
	void update() override;
	void attack(int dir);

private:
	Transform* tr_;
	RootSpawner* rootSpawner_;

	Uint32 lastTime_;
	bool attacking_;
	float rootPos_;
	int dir_;
	int rootW_;

	int timeBetweenRoots_;
};

