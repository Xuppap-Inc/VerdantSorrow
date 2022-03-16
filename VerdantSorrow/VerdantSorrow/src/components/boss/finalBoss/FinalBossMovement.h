#pragma once
#pragma once
#include "../../../ecs/Component.h"
#include "../../../utils/Vector2D.h"
#include <SDL.h>

class Transform;
class BossAtributos;
class HandsManager;

class FinalBossMovement : public ecs::Component
{
public:
	enum Phase { PHASE1, PHASE2 };
	enum EyeState { BOUNCE, GROUND, };

	__CMPID_DECL__(ecs::_BOSS_ATTACK_MANAGER)
		FinalBossMovement();
	~FinalBossMovement();
	void update() override;
	void bounce();
	void initComponent()override;
private:
	Transform* tr_;
	BossAtributos* bA_;
	HandsManager* handMngr_;
	Phase phase_;
	EyeState eyeState_;

	float eyeSpeed;
};

