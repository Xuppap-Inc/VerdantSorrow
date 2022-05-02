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

    /**
    M�todo que se invoca cuando el jugador choca con el BORDE INFERIOR de la pantalla
    */
    virtual void onBottom() {};
    /**
    M�todo que se invoca cuando el jugador choca con el BORDE SUPERIOR de la pantalla
    */
    virtual void onTop() {};
    /**
    M�todo que se invoca cuando el jugador choca con el BORDE IZQUIERDO de la pantalla
    */
    virtual void onLeft() {};
    /**
    M�todo que se invoca cuando el jugador choca con el BORDE DERECHO de la pantalla
    */
    virtual void onRight() {};

    /**
    M�todo que se invoca cuando el jugador NO est� chocando con NINGUNO de los BORDES LATERALES de la pantalla
    */
    virtual void onNoLeftAndRight() {};
    /**
    M�todo que se invoca cuando el jugador NO est� chocando ni con el BORDE SUPERIOR ni el BORDE INFERIOR
    */
    virtual void onNoTopAndBot() {};

    void setActive(bool set) { active_ = set; };
    bool isActive() { return active_; };

protected:
    Transform* tr_;

    bool active_ = true;

    const int GROUND_HEIGHT = 130;
};