
#pragma once
#include "../../../ecs/Component.h"
#include "../../../game/CollisionManager.h"
#include "../../RectangleCollider.h"
#include "../../../sdlutils/SDLUtils.h"

#include <vector>
#include <iostream>
#include <SDL_stdinc.h>
#include <string>
class Transform;
class PlayerDeathZone :
    public ecs::Component
{
public:
    __CMPID_DECL__(ecs::_MUSHROOM)
        PlayerDeathZone(CollisionManager* colMan,std::string s);

    ~PlayerDeathZone();
    void initComponent() override;
    void update() override;
protected:
    Transform* tr_;
    CollisionManager* colMan_;
    RectangleCollider* collider_;
    std::string area_;
};
