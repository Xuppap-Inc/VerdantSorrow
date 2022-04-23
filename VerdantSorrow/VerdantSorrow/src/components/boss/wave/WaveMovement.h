#pragma once
#include "../../../ecs/Component.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../../utils/Vector2D.h"
//#include "../../../sdlutils/VirtualTimer.h"

class Transform;
//class VirtualTimer;
class WaveMovement : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_WAVEMOVEMENT)
	WaveMovement(Vector2D dir, float speed, bool particles = true);
	~WaveMovement();

	void initComponent() override;
	void update() override;

private:
	Transform* tr_;
	Vector2D dir_;
	float speed_;

	bool particles_;
};