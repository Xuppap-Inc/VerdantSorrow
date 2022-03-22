#include "../../../ecs/Component.h"
#include "../../../sdlutils/SDLUtils.h"

class FrogJump;
class FrogBigJump;
class Transform;
class TongueAttack;
class CollisionManager;
class BossAtributos;
class FramedImage;
class WaveSpawner;

#pragma once
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
		WAITING_FOR_TONGUE,
		FLY_DIED,
		SECOND_PHASE
	};

	FrogAttackManager();
	~FrogAttackManager();
	FrogAttackManager(CollisionManager* collManager);
	void initComponent() override;
	void update() override;
	ecs::Entity* createFly();
	ecs::Entity* createTongue(CollisionManager* colManager);
	void onFlyDied();
private:
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
	void flipOnBorders();
	void onGrounded(bool &jump, bool isBig);
	void nextAttack();
	FrogJump* frogJump_;
	FrogBigJump* bigJump_;
	ecs::Entity* fly_;
	ecs::Entity* tongue_;
	Transform* player_;
	Transform* tr_;
	CollisionManager* collManager_;
	BossAtributos* attr_;
	FramedImage* anim_;
	SoundEffect* musicaFase1_;
	Music* musicaFase2_;

	//clase para spawnear waves
	WaveSpawner* waveSp_;

	State frogState_;
	AnimState animState_;
	AnimState animNewState_;
	bool secondPhase_;
	bool angry_;
	bool jumping_;
	int jumpDirection_;	
	int jumpsUntilNextTongue_;
	int flySpacing_;
	bool attacking_;
	bool jumpingBig_;
	Uint32 delay_;
	Uint32 lastUpdate_;
	Uint32 tongueDelay_;

};

