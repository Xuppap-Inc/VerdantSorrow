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

    void setIsTrigger(bool set);
    bool isTrigger();

    void setActive(bool set);
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

