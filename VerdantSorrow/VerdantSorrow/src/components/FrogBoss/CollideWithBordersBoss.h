#pragma once
#include "../../ecs/Component.h"
#include "../../ecs/ecs.h"

class Transform;

class CollideWithBordersBoss :
    public ecs::Component
{
public:
    __CMPID_DECL__(ecs::_COLLIDEWITHBORDERS)
        CollideWithBordersBoss();
    ~CollideWithBordersBoss();
    void initComponent() override;

    void update() override;

protected:
    Transform* tr_;
};

