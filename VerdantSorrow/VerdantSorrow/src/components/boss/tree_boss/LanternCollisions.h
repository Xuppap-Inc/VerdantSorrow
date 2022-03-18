#pragma once
#include "../../../ecs/Component.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../../game/CollisionManager.h"

class Transform;
class BossAtributos;
class RectangleCollider;

class LanternCollisions: public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_LANTERN_COLLISIONS)

	LanternCollisions(CollisionManager* colManager);
	~LanternCollisions();
	void initComponent()override;
	virtual void update()override;

	void setDamaged(bool set);

private:
	Transform* lanternTr_;
	BossAtributos* attrib_;
	RectangleCollider* collider_;

	CollisionManager* colMan_;

	bool damaged_;
};

