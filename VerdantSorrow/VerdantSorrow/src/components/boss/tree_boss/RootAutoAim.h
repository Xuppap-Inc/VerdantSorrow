#pragma once
#include "../../../ecs/Component.h"
#include "RootSpawner.h"
#include "../../../sdlutils/VirtualTimer.h"

class TreeMovement;
class VirtualTimer;

class RootAutoAim : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_ROOTAUTOAIM)

	RootAutoAim(ecs::Entity* player);
	~RootAutoAim();

	void initComponent() override;
	void update() override;
	void attack(bool infinite);
	void cancelAttack();

	void changeToSecondPhase();
	bool hasFinished() { return !attacking_; };

private:

	const int TIME_BETWEEN_ROOTS = 1000;
	const int DURATION = 7000;

	const int LANTER_SECURE_SPACE = 100;

	Transform* tr_;
	RootSpawner* rootSpawner_;
	TreeMovement* movement_;

	ecs::Entity* player_;
	Transform* playerTr_;

	Transform* lanternTr_;

	bool secondPhase_;

	VirtualTimer * lastTime_;
	VirtualTimer* iniTime_;
	bool attacking_;
	float rootPos_;
	int rootW_;

	bool infiniteDuration_;
};

