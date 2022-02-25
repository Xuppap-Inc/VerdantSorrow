#pragma once
#include "../../ecs/Component.h"
#include "../../game/CollisionManager.h"
#include "../RectangleCollider.h"
#include "PlayerAttributes.h"
#include "../SimpleGravity.h"
#include "../../sdlutils/SDLUtils.h"

#include <vector>
#include <iostream>
#include <SDL_stdinc.h>

class Transform;
class SimplePhysicsPlayer :
    public ecs::Component
{
public:
    __CMPID_DECL__(ecs::_PHYSICS)
    SimplePhysicsPlayer(CollisionManager* colMan);

    ~SimplePhysicsPlayer();
    void initComponent() override;

    void update() override;
protected:
    Transform* tr_;
    CollisionManager* colMan_;
    RectangleCollider* collider_;
    PlayerAttributes* attrib_;
    SimpleGravity* gravity_;
    bool invulnerable_;
    Uint32 invTimer;
};

