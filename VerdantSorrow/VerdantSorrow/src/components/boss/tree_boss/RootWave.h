#pragma once
#include "../../../ecs/Component.h"
#include "RootSpawner.h"
#include "LanternSpawner.h"
#include "../../../sdlutils/VirtualTimer.h"

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
	bool getMove() { return ableToMove_; }

	bool hasFinished() { return attacking_ == false; };

private:
	Transform* tr_;
	RootSpawner* rootSpawner_;
	TreeMovement* treeMovement_;
	LanternSpawner* lanternSpawner_;

	VirtualTimer* rootSpawnTimer_;
	VirtualTimer* waitTimer_;
	Uint32 movingTime_;
	bool attacking_;
	bool ableToMove_;
	float rootPos_;
	int dir_;
	int rootW_;
	int lanternPos_;
	
	const int SPACE_BETWEEN_ROOTS = 50;
	const int TIME_BETWEEN_ROOTS = 150;
	const int TIME_UNTIL_MOVING = 3000;
};

