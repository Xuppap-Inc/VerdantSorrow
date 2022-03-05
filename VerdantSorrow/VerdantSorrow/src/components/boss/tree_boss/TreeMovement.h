#pragma once
#include "../../../ecs/Component.h"
#include "../../../sdlutils/SDLUtils.h"
class Transform;

class TreeMovement : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_TREEMOVEMENT)

		TreeMovement();
	TreeMovement(Transform* playerTransform_, float offsetX, float offsetY, Vector2D velocity, float followVelocity);
	~TreeMovement();

	void initComponent() override;
	virtual void update() override;

private:
	Transform* tr_;
	Transform* playerTr_;
	float offsetX_;
	float offsetY_;
	float lastUpdate_;
	Vector2D vel_;
	float followVel_;
};

