#pragma once
#include "../../ecs/Component.h"
#include "../../game/CollisionManager.h"
#include "../RectangleCollider.h"
#include "PlayerAttributes.h"
#include "../SimpleGravity.h"
#include "PlayerCtrl.h"
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
    void gravedad(bool gr) {
        gravedad_ = gr;
    }
    void update() override;
    void onCollisionExit();
protected:
    Transform* tr_;
    CollisionManager* colMan_;
    RectangleCollider* collider_;
    PlayerAttributes* attrib_;
    SimpleGravity* gravity_;
    PlayerCtrl* ctrl_;
    bool invulnerable_;
    Uint32 invTimer;
    bool gravedad_;
    //cuando se deja de colisionar, se resetea lo necesario
    bool exitCollision;
    std::vector <bool> lastCollision;
};

