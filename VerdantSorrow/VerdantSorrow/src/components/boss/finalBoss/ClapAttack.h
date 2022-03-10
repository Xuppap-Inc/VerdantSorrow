#pragma once
#include "../../../ecs/Component.h"
#include "../../../utils/Vector2D.h"
#include "../../RectangleCollider.h"
#include "../../../game/CollisionManager.h"
#include <SDL.h>

class Transform;

class ClapAttack : public RectangleCollider
{
public:
    __CMPID_DECL__(ecs::_MELEEATTACK)
    
    ClapAttack(bool leftHand);
    ~ClapAttack();

    void initComponent() override;
    void update() override;
protected:
    Transform* tr_;
    RectangleCollider* collider_;
    bool leftHand_, movingToAttack = true;
    int handSpeed = 4;
    float timerToStart_ = 0, countdownToStart = 0;
};

