#pragma once
#include "PhysicsComponent.h"
#include "CollisionManager.h"
#include "RectangleCollider.h"

#include <vector>
#include <iostream>

class SimplePhysicsPlayer :
    public PhysicsComponent
{
public:
    SimplePhysicsPlayer(float gravity, CollisionManager* colManager, RectangleCollider* collider);

    ~SimplePhysicsPlayer();

    void update(Container* o) override;
protected:

    float gravity_;
    CollisionManager* colMan_;
    RectangleCollider* collider_;
};

