#pragma once
#include "../../../ecs/Component.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../../game/CollisionManager.h"

class Transform;
class BossAtributos;
class RectangleCollider;
class LanternMovement;
class Attack;

class LanternCollisions: public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_LANTERN_COLLISIONS)

	LanternCollisions(CollisionManager* colManager);
	~LanternCollisions();
	void initComponent()override;
	virtual void update()override;

	void setDamaged(bool set);
	void changeToSecondPhase();

private:

	const int ROOT_DAMAGE_TO_LAMP = 6;
	const int NUM_ATTACKS_TO_REPOSITION = 3;

	Transform* lanternTr_;
	BossAtributos* attrib_;
	RectangleCollider* collider_;

	CollisionManager* colMan_;

	LanternMovement* lanterMov_;

	Attack* playerAttack_;

	bool damaged_;

	bool attacked_;

	bool secondPhase_;
	int contAttacks_;
};

