#pragma once
#include "../../../ecs/Component.h"
#include "../../../game/CollisionManager.h"
#include "../../RectangleCollider.h"
#include "../../../sdlutils/SDLUtils.h"

#include <vector>
#include <iostream>
#include <SDL_stdinc.h>

class Transform;
class SimpleGravity;
class Spikes :
    public ecs::Component
{
public:
    __CMPID_DECL__(ecs::_MUSHROOM)
        Spikes(CollisionManager* colMan);

    ~Spikes();
    void initComponent() override;
    void update() override;
protected:
    Transform* tr_;
    SimpleGravity* gr_;
    CollisionManager* colMan_;
    RectangleCollider* collider_;
    bool moving_;
    int activationDistance_;
    float spikeVelocity_;
};
