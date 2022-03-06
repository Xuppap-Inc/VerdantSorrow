#pragma once
#include "../../../ecs/Component.h"
#include "../../../sdlutils/SDLUtils.h"
class Transform;

class TreeMovement : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_TREEMOVEMENT)

		TreeMovement();
	TreeMovement(Transform* playerTransform_, float offsetX, float followVelocity);
	~TreeMovement();

	void initComponent() override;
	virtual void update() override;

private:
	Transform* tr_;
	Transform* playerTr_;
	float offsetX_;
	float lastUpdate_;
	float followVel_;
};

