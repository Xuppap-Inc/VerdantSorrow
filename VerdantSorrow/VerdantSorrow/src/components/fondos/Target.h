#pragma once
#include "../../ecs/Component.h"
#include "../../sdlutils/VirtualTimer.h"
#include<vector>

class ecs::Manager;

class Transform;
class Image;

class Target : public ecs::Component {
public:

	__CMPID_DECL__(ecs::_TARGET)

		Target(int time, Transform* target);
	virtual ~Target();
	void initComponent() override;
	void update() override;
private:
	Transform* tr_;

	int time_;

	Image* img_;
	VirtualTimer timer_;

	Transform* target_;
};


