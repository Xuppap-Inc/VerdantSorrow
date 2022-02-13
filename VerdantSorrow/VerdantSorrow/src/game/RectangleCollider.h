#pragma once
#include "PhysicsComponent.h"
#include "../utils/Vector2D.h"
#include <SDL.h>

class RectangleCollider :
    public PhysicsComponent
{
public:
    RectangleCollider(float width, float height);
    ~RectangleCollider();

    void update(Container* o) override;

    SDL_Rect getCollider();

    void setIsTrigger(bool set);
    bool isTrigger();

    void setActive(bool set);
    bool isActive();
    
protected:
    float width_;
    float height_;

    Vector2D pos_;

    bool isTrigger_;
    bool active_;
};

