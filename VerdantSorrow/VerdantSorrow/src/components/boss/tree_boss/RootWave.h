#pragma once
#include "../../../ecs/Component.h"
#include "RootSpawner.h"
#include "LanternSpawner.h"

class TreeMovement;

class RootWave : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_ROOTWAVE)

	RootWave();
	~RootWave();

	void initComponent() override;
	void update() override;
	void attack(int dir);

	bool hasFinished() { return attacking_ == false; };

private:
	Transform* tr_;
	RootSpawner* rootSpawner_;
	TreeMovement* treeMovement_;
	LanternSpawner* lanternSpawner_;

	Uint32 lastTime_;
	bool attacking_;
	float rootPos_;
	int dir_;
	int rootW_;
	int lanternPos_;
	
	const int SPACE_BETWEEN_ROOTS = 10;
	const int TIME_BETWEEN_ROOTS = 150;
};

