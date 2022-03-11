#pragma once
#include "../../../ecs/Component.h"
#include "RootSpawner.h"

class TreeMovement;

class RootAutoAim : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_ROOTAUTOAIM)

	RootAutoAim(ecs::Entity* player);
	~RootAutoAim();

	void initComponent() override;
	void update() override;
	void attack();
	void cancelAttack();

	bool hasFinished() { return !attacking_; };

private:

	int TIME_BETWEEN_ROOTS = 1000;
	int DURATION = 7000;

	Transform* tr_;
	RootSpawner* rootSpawner_;
	TreeMovement* movement_;

	ecs::Entity* player_;
	Transform* playerTr_;

	Uint32 lastTime_;
	int iniTime_;
	bool attacking_;
	float rootPos_;
	int rootW_;
};

