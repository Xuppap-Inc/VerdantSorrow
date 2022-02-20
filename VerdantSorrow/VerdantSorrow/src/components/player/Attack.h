#pragma once
#include "../../ecs/Component.h"
#include "../../utils/Vector2D.h"
#include <SDL.h>

class Transform;
using ecs::Entity;

class Attack : public ecs::Component
{
public:
    __CMPID_DECL__(ecs::_ATTACK)
    Attack(float width, float height);
    ~Attack();

    void initComponent() override;
    void update() override;
    void render() override;

    SDL_Rect getCollider();

    /**
    Método que permite cambiar la variable active, que indica si el collider se encuentra activo
    *
    @param set: Valor que tomará active
    */
    void setActive(bool set);
    /**
    Método que indica si elcollider se encuentra activo
    *
    @return true si está activo, false en caso contrario
    */
    bool isActive();

    Entity* getEntity();
    
protected:
    Transform* tr_;
    float width_;
    float height_;

    Vector2D pos_;

    bool active_;

    //Variables que controlan el timing del ataque
    int attackDuration;
    int attackCoolDown;
    int lastAttack;

    /**
    * Setea la posicion del ataque delante del jugador, teniendo en cuenta su direccion de movimiento
    */
    void setPosition();
};

