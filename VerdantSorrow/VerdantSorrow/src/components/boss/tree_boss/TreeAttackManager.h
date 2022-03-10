#include "../../../ecs/Component.h"
#include "root/RootMovement.h"

class RootWave;
class RootAutoAim;
class MeleeAttack;
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
		MOVING,
		WAVE,
		SPECIAL_ATTACK
	};
	enum Phase {
		PHASE1,
		PHASE2
	};

	TreeAttackManager();
	~TreeAttackManager();
	TreeAttackManager(CollisionManager* collManager);
	void initComponent() override;
	void update() override;

	int getRootWidth() { return rootWidth_; }
private:

	const float MELEE_ATTACK_DISTANCE = 50.0;
	const int TIME_BETWEEN_WAVES = 5000;

	State state;
	Phase phase;

	Transform* player_;
	Transform* tr_;
	CollisionManager* collManager_;
	BossAtributos* attr_;
	FramedImage* anim_;

	RootWave* rootWave_;
	RootAutoAim* rootAutoAim_;
	MeleeAttack* meleeAttack_;
	
	SoundEffect* musicaFase1_;
	Music* musicaFase2_;

	int rootWidth_;
	int lastTimeWave_;

	bool attacking_;
};

