#pragma once
#include "../ecs/Component.h"
#include "../utils/Vector2D.h"
#include <SDL.h>

class Transform;
using ecs::Entity;

class RectangleCollider :
    public ecs::Component
{
public:
    __CMPID_DECL__(ecs::_COLLIDER)
    RectangleCollider(float width, float height);
    ~RectangleCollider();

    void initComponent() override;
    void update() override;

    SDL_Rect getCollider();

    /**
    Método que permite cambiar la variable isTrigger, que indica si el collider es un trigger
    *
    @param set: Valor que tomará isTrigger
    */
    void setIsTrigger(bool set);
    /**
    Método que indica si el collider es un trigger
    *
    @return true si es un trigger, false en caso contrario
    */
    bool isTrigger();

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

    bool isTrigger_;
    bool active_;
};

