#include "../../../ecs/Component.h"
#include "root/RootMovement.h"
#include "../../../sdlutils/VirtualTimer.h"

class RootWave;
class RootAutoAim;
class MeleeAttack;
class Transform;
class CollisionManager;
class FramedImage;
class Image;
class TreeMovement;
class LanternMovement;
class LanternCollisions;
class BossAtributos;

#pragma once
class TreeAttackManager : public ecs::Component
{

public:
	__CMPID_DECL__(ecs::_BOSS_ATTACK_MANAGER)
	enum State {
		MOVING,
		WAVE,
		SPECIAL_ATTACK,
		MOVING_TO_CENTER
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
	enum AnimState {
		ANIM_IDLE,
		ANIM_WALK,
		ANIM_ATTACK,
		ANIM_ATTACK_COMBO,
		ANIM_ROOTS,
		ANIM_BACKGROUND,
		ANIM_CHANGE_PHASE,
		ANIM_DEATH,
	};

	AnimState animState_;
	AnimState animNewState_;

	const float MELEE_ATTACK_DISTANCE = 50.0;
	const int TIME_BETWEEN_WAVES = 5000;

	const int TIME_FOR_SPECIAL = 10000;
	const int WAIT_AFTER_SPECIAL = 1000;

	State state;
	Phase phase;

	Transform* player_;
	Transform* tr_;
	CollisionManager* collManager_;
	FramedImage* anim_;
	RectangleCollider* treeCol_;

	TreeMovement* movement_;

	RootWave* rootWave_;
	RootAutoAim* rootAutoAim_;
	MeleeAttack* meleeAttack_;

	Entity* lantern_;
	Transform* lanternTr_;
	LanternMovement* lanternMov_;
	LanternCollisions* lanternCols_;
	BossAtributos* attribs_;
	
	SoundEffect* musicaFase1_;
	Music* musicaFase2_;

	int rootWidth_;
	VirtualTimer timerWave_;
	VirtualTimer timerSpecial_;
	VirtualTimer waitTimer_;

	bool attacking_;
	bool waiting_;

	bool newAtack_;
	int dir_;

	void returnToIni();

	void attackWave(int dir);
	void attackSpecial();
	void prepareToSpecial();
};

