#pragma once
#include "PhysicsComponent.h"

#include "PlayerCtrl.h"
class CollideWithBorders :
    public PhysicsComponent
{
public:
    CollideWithBorders(PlayerCtrl* controller);
    ~CollideWithBorders();
    void update(Container* o) override;

protected:
    PlayerCtrl* playerController_;
};

