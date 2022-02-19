#pragma once

#include "../../ecs/Component.h"

class PlayerAttributes :
    public ecs::Component
{
public:
    __CMPID_DECL__(ecs::_ATTRIBUTES)
    PlayerAttributes();
    ~PlayerAttributes();

    bool isOnGround();
    void setOnGround(bool set);

    bool isRightStop();
    void setRightStop(bool set);

    bool isLeftStop();
    void setLeftStop(bool set);

    void damagePlayer(int lives);

protected:

    int lives_;

    //Booleana que indica si el jugador está tocando el suelo
    bool onGround;

    //Booleana que indica si el jugador se puede mover hacia la derecha
    bool rightStop;
    //Booleana que indica si el jugador se puede mover hacia la izquierda
    bool leftStop;
};

