#pragma once
#include "../../ecs/Component.h"
#include "../../game/CollisionManager.h"
#include "../RectangleCollider.h"
#include "../SimpleGravity.h"
#include "BossAtributos.h"
#include "../../sdlutils/SDLUtils.h"

#include <vector>
#include <iostream>
#include <SDL_stdinc.h>

class Transform;
class CollisionsSwordFrog :
    public ecs::Component
{
public:
    __CMPID_DECL__(ecs::_PHYSICS)
        CollisionsSwordFrog(CollisionManager* colMan);

    ~CollisionsSwordFrog();
    void initComponent() override;

    void update() override;
protected:
    CollisionManager* colMan_;
    RectangleCollider* collider_;
    BossAtributos* attrib_;
    bool invulnerable_;
    Uint32 invTimer;
};

