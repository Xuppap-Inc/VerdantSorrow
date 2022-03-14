#pragma once
#pragma once
#include "../../../ecs/Component.h"
#include "../../../utils/Vector2D.h"
#include <SDL.h>

class Transform;
class FinalBossMovement : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_FINALBOSSMOVE)
	FinalBossMovement();
	~FinalBossMovement();
	void update() override;
	void initComponent()override;
private :
	Transform* tr_;


};

