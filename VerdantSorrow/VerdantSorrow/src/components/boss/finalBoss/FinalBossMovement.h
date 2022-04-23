#pragma once
#pragma once
#include "../../../ecs/Component.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../../utils/Vector2D.h"
#include <SDL.h>

class Transform;
class BossAtributos;
class HandsManager;
class CollisionManager;
class WaveSpawner;
class FramedImage;
class VirtualTimer;
class ParticleSystem;

class FinalBossMovement : public ecs::Component
{
public:
	enum Phase { PHASE1, PHASE2 };
	enum EyeState { BOUNCE, GROUND, };

	__CMPID_DECL__(ecs::_BOSS_ATTACK_MANAGER)
		FinalBossMovement(CollisionManager* colManager);
	~FinalBossMovement();
	void update() override;
	void bounce();
	void restartBouncing();
	void fireBall();
	void initComponent()override;
private:
	Transform* tr_;
	BossAtributos* bA_;
	HandsManager* handMngr_;
	FramedImage* anim_;
	Phase phase_;
	EyeState eyeState_;
	CollisionManager* colManager_;
	float eyeSpeed;
	Vector2D velocitySaved;
	WaveSpawner* waveSp_;
	Transform* playerTr_;

	SoundEffect* musicaFase1_;
	Music* musicaFase2_;

	VirtualTimer* lastTimeInGround_;
	//random bola de fuego times
	//int lastFireBall_;//, fireBallCooldown_;
	int /*lastTimeGround = 0,*/ timeInGround = 1000;
	
	ParticleSystem* ashes_;

	bool deadBoss_;
};

