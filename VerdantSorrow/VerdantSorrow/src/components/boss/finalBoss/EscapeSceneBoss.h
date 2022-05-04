#pragma once
#include "../../../ecs/Component.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../../utils/Vector2D.h"
#include <SDL.h>

class Transform;
class BossAtributos;
class CollisionManager;
class FramedImage;
class VirtualTimer;
class ParticleSystem;

class EscapeSceneBoss :public ecs::Component
{
public:
	enum EyeState { BOUNCE, GROUND, };

	__CMPID_DECL__(ecs::_BOSS_ATTACK_MANAGER)
		EscapeSceneBoss(CollisionManager* colManager);
	~EscapeSceneBoss();
	void update() override;
	void initComponent()override;

	void setActive(bool set) { active_ = set; };
	bool isActive() { return active_; };

	void setReturningToCenter(bool set) { returningToCenter = set; };
	bool isReturningToCenter() { return returningToCenter; };

	void setDeadBoss(bool set);

	int* getMusicVolume() { return &musicVolume_; };

	Music* getMusic() { return musica_; };
private:

	const int WAIT_ON_GROUND = 1000;

	const int RETURNING_SPEED = 5;

	Transform* tr_;
	BossAtributos* bA_;
	FramedImage* anim_;
	CollisionManager* colManager_;
	EyeState eyeState_;
	float eyeSpeed_;
	Vector2D velocitySaved;
	Transform* playerTr_;

	Music* musica_;

	VirtualTimer* lastTimeInGround_;

	ParticleSystem* ashes_;

	bool deadBoss_;

	bool active_ = true;;

	void bounce();
	void restartBouncing();

	bool returningToCenter = false;
	int musicVolume_;
};


