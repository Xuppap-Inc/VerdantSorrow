#pragma once
#include "../../../ecs/Component.h"
#include "../../../utils/Vector2D.h"
#include "../../RectangleCollider.h"
#include "../../../game/CollisionManager.h"
#include <SDL.h>
#include "../../../sdlutils/VirtualTimer.h"

class TreeMovement;
class Transform;
using ecs::Entity;

class MeleeAttack : public RectangleCollider
{
public:
    __CMPID_DECL__(ecs::_MELEEATTACK)
        MeleeAttack(float width, float height, CollisionManager* colManager);
    ~MeleeAttack();

    void initComponent() override;
    void update() override;
    void render() override;

    void attack(int dir);

    bool hasFinished() { return attacking_ == false; };

protected:
    Transform* tr_;

    TreeMovement* treeMovement_;
    //Variables que controlan el timing del ataque
    int attackDuration;
    int attackCoolDown;

    VirtualTimer* attackTimer_;

    CollisionManager* colMan_;

    /**
    * Setea la posicion del ataque delante del arbol, teniendo en cuenta su direccion de movimiento
    */
    void setPosition(int dir) ;

    bool attacking_;
};

