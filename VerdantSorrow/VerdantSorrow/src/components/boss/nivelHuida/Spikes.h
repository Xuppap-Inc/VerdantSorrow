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
    __CMPID_DECL__(ecs::_SPIKES)
        Spikes(CollisionManager* colMan,bool move);

    ~Spikes();
    void setmove(bool s) {
        movement_ = s;
    }
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
    bool movement_;
};
