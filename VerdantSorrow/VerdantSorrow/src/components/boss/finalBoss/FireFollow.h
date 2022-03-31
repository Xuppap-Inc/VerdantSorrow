#pragma once
#include "../../../ecs/Component.h"

class Transform;

class FireFollow :
	public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_FIREFOLLOW)
		FireFollow(ecs::Entity* hand);
	~FireFollow();
	void initComponent()override;
	
	void update()override;
private:
	Transform* tr_;
	ecs::Entity* hand_;
	Transform* handTr_;

};

