#pragma once
#include "../ecs/Component.h"
#include <bitset>

class Transform;
class ScrollCamera : public ecs::Component
{

public:
	__CMPID_DECL__(ecs::_CAMERA)
	ScrollCamera();
	ScrollCamera(float cameraSpeed, float deadzoneX = 120, float deadzoneY = 60);
	virtual ~ScrollCamera();
	void update() override;
	void initComponent() override;
	void debug() override;
	void setScrollX(bool x) {
		scrollX_ = x;
	}
protected:
	void calculateDirection();
	Transform* tr_;
	Transform* player_;
	float cameraSpeed_;
	float deadzoneX_, deadzoneY_;
	bool scrollX_;
};

