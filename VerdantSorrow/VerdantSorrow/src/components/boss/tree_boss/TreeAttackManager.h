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
class ParticleSystem;

#pragma once
class TreeAttackManager : public ecs::Component
{

public:
	__CMPID_DECL__(ecs::_BOSS_ATTACK_MANAGER)
	enum State {
		MOVING,
		WAVE,
		SPECIAL_ATTACK,
		MOVING_TO_CENTER,
		CHANGING_PHASE,
		DOING_ANIM,
		START_ANIM,
		DYING
	};
	enum Phase {
		PHASE1,
		PHASE2
	};

	TreeAttackManager(CollisionManager* collManager);
	~TreeAttackManager();

	void initComponent() override;
	void update() override;

	void checkIfDead();

	void checkDirection(float distance);

	void checkState(float absDistance);

	void checkAnimState();

	void deactivateBoss();

	int getRootWidth() { return rootWidth_; }

	bool isSecondPhase();

	int* getMusicVolume() { return &musicVolume_; };

	Music* getMusicSecondPhase() { return musicaFase2_; };
	SoundEffect* getMusicFirtPhase() { return musicaFase1_; };

private:
	enum AnimState {
		ANIM_IDLE,
		ANIM_WALK,
		ANIM_ATTACK,
		ANIM_ATTACK_COMBO,
		ANIM_ROOTS,
		ANIM_BACKGROUND,
		ANIM_CHANGE_PHASE,
		ANIM_DEATH
	};

	AnimState animState_;
	AnimState animNewState_;

	const float MELEE_ATTACK_DISTANCE = 20.0;
	const int TIME_BETWEEN_WAVES = 5000;

	const int TIME_FOR_SPECIAL = 10000;
	const int WAIT_AFTER_SPECIAL = 1000;
	const int ATTACK_CD = 2000;

	//tiempo que espera al inicio del combate
	const int START_DELAY = 2000;

	const int DEATH_DELAY = 10000;

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

	ParticleSystem* hojas_;

	int rootWidth_;
	
	VirtualTimer* timerWave_;
	VirtualTimer* timerSpecial_;
	VirtualTimer* waitTimer_;
	VirtualTimer* timerCd_;

	VirtualTimer* startTimer_;
	VirtualTimer* deathTimer_;

	bool attacking_;
	bool waiting_;

	bool newAtack_;
	int dir_;

	void returnToIni();

	void attackWave();
	void attackSpecial();
	void prepareToSpecial();

	void checkPhaseChange();

	bool deadBoss_;

	ParticleSystem* deadParticles1_;
	ParticleSystem* deadParticles2_;

	int musicVolume_;
};

