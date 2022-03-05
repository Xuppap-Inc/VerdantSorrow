#pragma once
#include "../../ecs/Component.h"
#include "../../sdlutils/SDLUtils.h"
class Transform;

class FollowPlayer: public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_FOLLOWPLAYER)

	FollowPlayer();
	FollowPlayer(Transform * playerTransform_, float offsetX, float offsetY, Vector2D velocity, float followVelocity);
	~FollowPlayer();

	void initComponent() override;
	void update() override;

private:
	Transform* tr_;
	Transform* playerTr_;
	float offsetX_;
	float offsetY_;
	float lastUpdate_;
	Vector2D vel_;
	float followVel_;
};

