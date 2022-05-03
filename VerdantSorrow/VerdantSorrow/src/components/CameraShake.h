#pragma once
#include "../ecs/Component.h"

class Transform;

class CameraShake : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_CAMERASHAKE)
	CameraShake(float shakeRad, int shakeDur);
	virtual ~CameraShake() {};
	void initComponent() override;
	void update() override;
	void shake(float,float);
protected:
	float shakeSp_;
	Transform* tr_;
	int shakeDur_;
	float shakeRadius_;
	int iteration_;
	bool shaking_;
};

