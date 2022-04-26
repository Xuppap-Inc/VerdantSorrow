#pragma once
#include "../../../ecs/Component.h"
#include "../../../game/CollisionManager.h"
#include "../../RectangleCollider.h"
#include "../../../sdlutils/SDLUtils.h"

#include <vector>
#include <iostream>
#include <SDL_stdinc.h>

class Transform;
class FlySpawnerPlant :
    public ecs::Component
{
public:
    __CMPID_DECL__(ecs::_FLYSPAWNERPLANT)
        FlySpawnerPlant(CollisionManager* colMan, bool lookingUp, float timer);

    ~FlySpawnerPlant();
    void initComponent() override;
    void update() override;
protected:
    float cooldownSpawner_;
    bool lookingUp_;
    VirtualTimer* vt;
    Transform* tr_;
    CollisionManager* colMan_;
};
