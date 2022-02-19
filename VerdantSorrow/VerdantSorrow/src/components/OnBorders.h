#pragma once
#include "../ecs/Component.h"
#include "../ecs/ecs.h"

class Transform;

class OnBorders :
    public ecs::Component
{
public:
    __CMPID_DECL__(ecs::_ONBORDERS)
    OnBorders();
    ~OnBorders();
    void initComponent() override;

    void update() override;

    virtual void onBottom() {};
    virtual void onTop() {};
    virtual void onLeft() {};
    virtual void onRight() {};

    virtual void onNoLeftAndRight() {};
    virtual void onNoTopAndBot() {};

protected:
    Transform* tr_;
};

