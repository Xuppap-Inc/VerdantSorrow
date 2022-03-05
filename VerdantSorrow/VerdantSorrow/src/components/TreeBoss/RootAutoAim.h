#pragma once
#include "../../ecs/Component.h"
#include "RootSpawner.h"

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

private:
	Transform* tr_;
	RootSpawner* rootSpawner_;

	ecs::Entity* player_;
	Transform* playerTr_;

	Uint32 lastTime_;
	bool attacking_;
	float rootPos_;
	int rootW_;

	int timeBetweenRoots_;
};

