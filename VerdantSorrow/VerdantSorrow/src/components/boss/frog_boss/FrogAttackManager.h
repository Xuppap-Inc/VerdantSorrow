#pragma once

#include "../../../ecs/Component.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../../sdlutils/VirtualTimer.h"

class FrogJump;
class FrogBigJump;
class Transform;
class TongueAttack;
class CollisionManager;
class BossAtributos;
class FramedImage;
class WaveSpawner;
class VitualTimer;
class ParticleSystem;

class FrogAttackManager : public ecs::Component
{

public:
	__CMPID_DECL__(ecs::_BOSS_ATTACK_MANAGER)

	static enum State {
		FIRST_PHASE,
		CALC_NEXT_ATTACK,
		JUMPING,
		JUMPING_BIG,
		TONGUE,
		WAITING,
		CASTING_TONGUE,
		FLY_DIED,
		SECOND_PHASE,
		DOING_ANIMATION,
		START_ANIMATION,
		DYING
	};

	FrogAttackManager(CollisionManager* collManager); 
	~FrogAttackManager();

	void initComponent() override;
	void update() override;
	ecs::Entity* createFly();
	ecs::Entity* createTongue(CollisionManager* colManager);
	void onFlyDied();

	void deactivateBoss();

private:

	const int FLOOR_HEIGHT = 100;

	//tiempo que dura la animacion de inicio con la rana quieta
	const int START_DELAY = 5000;
	const int DEATH_DELAY = 5000;

	enum AnimState {
		ANIM_IDLE,
		ANIM_JUMP,
		ANIM_BIG_JUMP,
		ANIM_TONGUE,
		ANIM_CHANGE_PHASE,
		ANIM_JUMP_TO_VULNERABLE,
		ANIM_VULNERABLE,
		ANIM_VULNERABLE_TO_IDLE,
		ANIM_DEATH,
	};

	void checkJumpDirection();
	void onGrounded(bool &jump, bool isBig);
	void nextAttack();

	void checkPhaseChange();
	void dieAnimation();
	void checkAnimationState();
	void checkFrogState();
	void checkIfDead();
	void checkPositionPlayer();

	FrogJump* frogJump_;
	FrogBigJump* bigJump_;
	ecs::Entity* fly_;
	ecs::Entity* tongue_;
	Transform* player_;
	Transform* tr_;
	CollisionManager* collManager_;
	BossAtributos* attr_;
	FramedImage* anim_;
	FramedImage* tongueAnim_;
	SoundEffect* musicaFase1_;
	Music* musicaFase2_;

	ParticleSystem* dandellions1_;
	ParticleSystem* dandellions2_;

	//clase para spawnear waves
	WaveSpawner* waveSp_;

	State frogState_;
	AnimState animState_;
	AnimState animNewState_;
	bool secondPhase_;
	bool jumping_;
	int jumpDirection_;
	int oldJumpDirection_;
	int jumpsUntilNextTongue_;
	int flySpacing_;
	bool attacking_;
	bool jumpingBig_;
	Uint32 delay_;

	VirtualTimer* vt_;
	VirtualTimer* startTimer_;
	VirtualTimer* deathTimer_;

	bool deadBoss_;

	ParticleSystem* deadParticles1_;
	ParticleSystem* deadParticles2_;
};

