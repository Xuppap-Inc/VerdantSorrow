#pragma once

#include "../../ecs/Component.h"

class PlayerAttributes :
    public ecs::Component
{
public:
    __CMPID_DECL__(ecs::_PLAYERATTRIBUTES)
    PlayerAttributes();
    ~PlayerAttributes();

    bool isOnGround();
    void setOnGround(bool set);

protected:

    int lives;
    bool onGround;
};

