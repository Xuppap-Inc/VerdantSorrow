#pragma once
#include "../ecs/Component.h"
#include "../ecs/ecs.h"

#include "PlayerCtrl.h"

class Transform;

class CollideWithBorders :
    public ecs::Component
{
public:
    __CMPID_DECL__(ecs::_COLLIDEWITHBORDERS)
    CollideWithBorders();
    ~CollideWithBorders();
    void initComponent() override;

    void update() override;

protected:
    Transform* tr_;
    PlayerCtrl* playerController_;
};

