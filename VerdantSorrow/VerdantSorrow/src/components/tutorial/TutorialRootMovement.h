#pragma once
#include "../../ecs/Component.h"
#include "../../sdlutils/SDLUtils.h"
#include "../RectangleCollider.h"
#include "../../sdlutils/VirtualTimer.h"

class Transform;

class TutorialRootMovement : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_TUTORIALROOTMOVEMENT)
		TutorialRootMovement();
	~TutorialRootMovement();

	void initComponent() override;
	void update() override;

private:
	Transform* tr_;
	RectangleCollider* col_;
	float speed_;
	VirtualTimer* rootSpawnTimer_;
};