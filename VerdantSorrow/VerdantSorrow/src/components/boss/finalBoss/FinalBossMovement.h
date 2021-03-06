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
	void initComponent()override;

	void setActive(bool set) { active_ = set; };
	bool isActive() { return active_; };

	void setReturningToCenter(bool set) { returningToCenter = set; };
	bool isReturningToCenter() { return returningToCenter; };

	void setDeadBoss(bool set);
	//Para ponerle la fase en el nivel de huida
	void setPhase(Phase p);

	int* getMusicVolume() { return &musicVolume_; };

	Music* getMusic() { return musica_; };
private:

	const int WAIT_ON_GROUND = 1000;

	const int RETURNING_SPEED = 5;

	Transform* tr_;
	BossAtributos* bA_;
	FramedImage* anim_;
	Phase phase_;
	EyeState eyeState_;
	CollisionManager* colManager_;
	float eyeSpeed_;
	Vector2D velocitySaved;
	WaveSpawner* waveSp_;
	Transform* playerTr_;

	Music* musica_;

	VirtualTimer* lastTimeInGround_;
	
	ParticleSystem* ashes_;

	bool deadBoss_;

	bool active_ = true;;

	void checkPhaseChange();
	void bounce();
	void restartBouncing();
	void fireBall();

	bool returningToCenter = false;
	int musicVolume_;
};

