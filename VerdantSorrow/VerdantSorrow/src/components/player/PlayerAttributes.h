#pragma once

#include "../../ecs/Component.h"

class PlayerAttributes :
    public ecs::Component
{
public:
    __CMPID_DECL__(ecs::_ATTRIBUTES)
    PlayerAttributes();
    ~PlayerAttributes();

    /**
    M�todo que indica si el jugador se encuentra en el SUELO
    *
    @return true si se encuentra en el suelo, false en caso contrario
    */
    bool isOnGround();
    /**
    M�todo que permite cambiar la variable onGround, que indica si el jugador se encuentra en el suelo
    *
    @param set: Valor que tomar� onGroud
    */
    void setOnGround(bool set);

    /**
    M�todo que indica si el jugador no puede ir a la DERECHA
    *
    @return true si tiene bloqueada la acci�n de moverse hacia la derecha, false en caso contrario
    */
    bool isRightStop();
    /**
    M�todo que permite cambiar la variable rightStop, que indica si el jugador no puede ir a la DERECHA
    *
    @param set: Valor que tomar� rightStop
    */
    void setRightStop(bool set);

    /**
    M�todo que indica si el jugador no puede ir a la IZQUIERDA
    *
    @return true si tiene bloqueada la acci�n de moverse hacia la izquierda, false en caso contrario
    */
    bool isLeftStop();
    /**
    M�todo que permite cambiar la variable leftStop, que indica si el jugador no puede ir a la IZQUIERDA
    *
    @param set: Valor que tomar� leftStop
    */
    void setLeftStop(bool set);

    /**
    M�todo que resta una cantidad determinada de vidas al jugador
    *
    @param lives: Vidas que perder� el jugador
    */
    void damagePlayer(int lives);

    /**
    M�todo que indica las vidas del jugador
    *
    @return las vidas del jugador actuales
    */
    int getLives();
protected:

    int lives_;

    //Booleana que indica si el jugador est� tocando el suelo
    bool onGround;

    //Booleana que indica si el jugador se puede mover hacia la derecha
    bool rightStop;
    //Booleana que indica si el jugador se puede mover hacia la izquierda
    bool leftStop;
};

