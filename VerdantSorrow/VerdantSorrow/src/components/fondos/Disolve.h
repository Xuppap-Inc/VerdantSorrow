#pragma once
#include "../../ecs/Component.h"
#include "../../sdlutils/VirtualTimer.h"
#include<vector>

class ecs::Manager;

class Transform;
class Image;

class Disolve : public ecs::Component {
public:

	__CMPID_DECL__(ecs::_DISOLVE)

		Disolve(int startFadingTime);
	virtual ~Disolve();
	void initComponent() override;
	void update() override;
private:
	Transform* tr_;

	int startFadingTime_;

	Image* img_;
	VirtualTimer* timer_;
};


