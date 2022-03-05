#include "../../../ecs/Component.h"

class FrogJump;
class FrogBigJump;
class Transform;
class TongueAttack;
class CollisionManager;
class BossAtributos;
class FramedImage;

#pragma once
class FrogAttackManager : public ecs::Component
{

public:
	__CMPID_DECL__(ecs::_BOSS_ATTACK_MANAGER)
	enum State {
		FIRST_PHASE,
		FLY_DIED,
		SECOND_PHASE
	};
	FrogAttackManager();
	~FrogAttackManager();
	FrogAttackManager(CollisionManager* collManager);
	void initComponent() override;
	void update() override;
	ecs::Entity* createFly();
	void createWave(int dir);
	void createWaves();
	void onFlyDied();
private:
	FrogJump* frogJump_;
	FrogBigJump* bigJump_;
	TongueAttack* tongueAttack_;
	ecs::Entity* fly_;
	Transform* player_;
	Transform* tr_;
	CollisionManager* collManager_;
	BossAtributos* attr_;
	FramedImage* anim_;
	
	State frogState_;
	bool jumping_;
	int jumpDirection_;	
	int jumpsUntilNextTongue_;
	int flySpacing_;
	bool attacking_;
	bool jumpingBig_;
};

