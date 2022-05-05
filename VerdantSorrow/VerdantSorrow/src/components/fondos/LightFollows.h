#pragma once
#include "../../ecs/Component.h"
#include "../../sdlutils/VirtualTimer.h"
#include<vector>

class ecs::Manager;

class Transform;
class Image;

class LightFollows : public ecs::Component {
public:

	__CMPID_DECL__(ecs::_LIGHT_FOLLOWS)

		LightFollows(Transform* follows);
	virtual ~LightFollows();
	void initComponent() override;
	void update() override;
private:
	Transform* tr_;
	Transform* follows_;
};


