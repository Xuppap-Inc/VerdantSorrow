#pragma once
#include "../ecs/Component.h"

class Transform;
class SimpleMove :
	public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_SIMPLEMOVE)
	SimpleMove();
	virtual ~SimpleMove();
	void initComponent() override;
	void update() override;
private:
	Transform* tr_;
};

