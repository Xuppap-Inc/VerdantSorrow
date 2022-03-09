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

    RectangleCollider();
    RectangleCollider(float width, float height);
    ~RectangleCollider();

    void initComponent() override;
    void update() override;

    void debug() override;

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

    void drawCollider();


    Entity* getEntity();

    Vector2D getPos() { return pos_; };
    float getWidth() { return width_; };
    float getHeight() { return height_; };
    void setWidth(float width) {  width_=width; };
    void setHeight(float height) { height_ = height; };
    
protected:

    //posiciona al collider en la posicion que le corresponde
    virtual void setPosition();

    Transform* tr_;
    float width_;
    float height_;

    Vector2D pos_;

    bool isTrigger_;
    bool active_;
};

