#pragma once
#include "../ecs/Component.h"
#include "RectangleCollider.h"

#include <vector>
#include <iostream>

class Transform;
class SimpleGravity :
    public ecs::Component
{
public:
    __CMPID_DECL__(ecs::_GRAVITY)
    SimpleGravity(float gravity);

    ~SimpleGravity();
    void initComponent() override;

    void update() override;

    void setActive(bool set);
    bool isActive();

protected:
    Transform* tr_;
    float gravity_;

    bool active_;
};

