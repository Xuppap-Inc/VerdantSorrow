#pragma once
#include "../../../ecs/Component.h"
#include "../../../sdlutils/SDLUtils.h"
class Transform;

class TreeMovement : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_TREEMOVEMENT)

		TreeMovement();
	TreeMovement(Transform* playerTransform_, float followVelocity);
	~TreeMovement();

	int getMovementDir() { return movementDir_; }
	bool isNextToPlayer() { return movementDir_ == 0; }
	void initComponent() override;
	virtual void update() override;

	bool isMoveActive() { return moveActive_; }
	void setMoveActive(bool set) { moveActive_ = set; }

private:
	Transform* tr_;
	Transform* playerTr_;
	float offsetX_;
	float followVel_;
	int movementDir_;
	bool isNextToPlayer_;

	bool moveActive_;
};

