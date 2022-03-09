#pragma once
#include "../../../ecs/Component.h"
#include "../../../utils/Vector2D.h"
#include "../../RectangleCollider.h"
#include "../../../game/CollisionManager.h"
#include <SDL.h>

class Transform;
using ecs::Entity;

class ClapAttack : public RectangleCollider
{
public:
    __CMPID_DECL__(ecs::_MELEEATTACK)
    
    ClapAttack(Entity* leftHand, Entity* rightHand);
    ~ClapAttack();

    void initComponent() override;
    void update() override;
protected:
    Entity* leftHand_;
    Entity* rightHand_;
};

