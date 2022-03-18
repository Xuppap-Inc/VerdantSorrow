#pragma once
#include "../../ecs/Component.h"
#include "PlayerAttributes.h"
#include "../RectangleCollider.h"
#include "../../game/CollisionManager.h"
#include <SDL.h>
class Transform;
class PlayerHubControl: public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_PLAYERCTRL)
		PlayerHubControl(float speed, CollisionManager* colManager);
	virtual ~PlayerHubControl() {};
	void update() override;
	void initComponent() override;
	void handleInput();

private:
	RectangleCollider* playerCol_;
	Transform* tr_;
	float speed_;
	PlayerAttributes* attrib_;
	CollisionManager* colMan_;

	bool moveLeft_, moveRight_, moveUp_, moveDown_;
};
