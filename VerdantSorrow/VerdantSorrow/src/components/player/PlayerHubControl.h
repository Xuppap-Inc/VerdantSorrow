#pragma once
#include "../../ecs/Component.h"
#include "PlayerAttributes.h"
#include "../RectangleCollider.h"
#include <SDL.h>
class Transform;
class PlayerHubControl: public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_PLAYERCTRL)
		PlayerHubControl( float speed) : speed_(speed),tr_(nullptr), attrib_() {}
	virtual ~PlayerHubControl();
	void update() override;
	void initComponent() override;

private:
	RectangleCollider* playerCol_;
	Transform* tr_;
	float speed_;
	PlayerAttributes* attrib_;
};
