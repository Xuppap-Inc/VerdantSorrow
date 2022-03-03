#include "../../ecs/Component.h"
#include "root/RootMovement.h"

class RootWave;
class Transform;
class CollisionManager;
class BossAtributos;
class FramedImage;

#pragma once
class TreeAttackManager : public ecs::Component
{

public:
	__CMPID_DECL__(ecs::_BOSS_ATTACK_MANAGER)
	enum State {
		FIRST_PHASE,
		FLY_DIED,
		SECOND_PHASE
	};
	TreeAttackManager();
	~TreeAttackManager();
	TreeAttackManager(CollisionManager* collManager);
	void initComponent() override;
	void update() override;

	int getRootWidth() { return rootWidth_; }
private:
	Transform* player_;
	Transform* tr_;
	CollisionManager* collManager_;
	BossAtributos* attr_;
	FramedImage* anim_;
	RootWave* rootWave_;
	
	int rootWidth_;
};

