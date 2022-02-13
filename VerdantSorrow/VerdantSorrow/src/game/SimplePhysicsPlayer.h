#pragma once
#include "PhysicsComponent.h"

class SimplePhysicsPlayer :
    public PhysicsComponent
{
public:
    SimplePhysicsPlayer(float gravity);

    ~SimplePhysicsPlayer();

    void update(Container* o) override;
protected:

    float gravity_;
};

