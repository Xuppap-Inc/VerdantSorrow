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

private:


	Transform* tr_;
	
	CollisionManager* colManager_;

	float eyeSpeed_;
	Vector2D velocitySaved;


	bool deadBoss_;

	bool active_ = true;;

	void bounce();

	bool returningToCenter = false;
	int musicVolume_;

};


