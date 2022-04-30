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
	void setScrollY(bool y) {
		scrollY_ = y;
	};

	void lock(bool x, bool y) {
		lockX_ = x;
		lockY_ = y;
		scrollX_ = !lockX_;
		scrollY_ = !lockY_;
	}
	void setLimitToDimensions(float x, float y, float w, float h) {
		limitLeft_ = x;
		limitTop_ = y;
		limitRight_ = x + w;
		limitBot_ = y + h;
	};
	void setLimitsToPositions(float right, float left, float top, float bot) {
		limitRight_ = right;
		limitLeft_ = left;
		limitTop_ = top;
		limitBot_ = bot;
	};
protected:
	void calculateDirection();
	Transform* tr_;
	Transform* player_;
	float cameraSpeed_;
	float deadzoneX_, deadzoneY_;
	bool scrollX_, scrollY_;
	float limitRight_, limitLeft_, limitTop_, limitBot_;
	bool lockX_, lockY_;
};

