#pragma once
#include "../../ecs/Component.h"
#include "../../ecs/Entity.h"

class PlatformAtribsForHub : public ecs::Component
{

public:
	__CMPID_DECL__(ecs::_PLATFORMATRIBSFORHUB)
		PlatformAtribsForHub(std::string sceneName); 
	~PlatformAtribsForHub() {};
	void initComponent()override;
	std::string getName() { return sceneName_; }
protected:
	std::string sceneName_;
	int scene;
};

