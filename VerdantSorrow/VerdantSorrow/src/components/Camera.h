#pragma once
#include "../ecs/Component.h"
#include <bitset>

class Transform;
class Camera : public ecs::Component
{

public:
	__CMPID_DECL__(ecs::_CAMERA)
	Camera();
	Camera(float followDistance, float cameraSpeed, float limitRight = -1, float limitLeft = 1, float limitTop = 1, float limitBot = -1);
	virtual ~Camera();
	void update() override;
	void initComponent() override;
protected:
	void calculateDirection();
	void moveObjects();
	Transform* tr_;
	Transform* player_;
	float followDistance_;
	float cameraSpeed_;
	int limitRight_, limitLeft_, limitTop_, limitBot_;
	std::bitset<4> followDirections_;
};

