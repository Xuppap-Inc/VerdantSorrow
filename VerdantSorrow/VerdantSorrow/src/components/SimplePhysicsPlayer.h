#pragma once
#include "../ecs/Component.h"
#include "../game/CollisionManager.h"
#include "RectangleCollider.h"

#include <vector>
#include <iostream>

class Transform;
class SimplePhysicsPlayer :
    public ecs::Component
{
public:
    __CMPID_DECL__(ecs::_PHYSICS)
    SimplePhysicsPlayer(float gravity, CollisionManager* colMan);

    ~SimplePhysicsPlayer();
    void initComponent() override;

    void update() override;
protected:
    Transform* tr_;
    float gravity_;
    CollisionManager* colMan_;
    RectangleCollider* collider_;
};

