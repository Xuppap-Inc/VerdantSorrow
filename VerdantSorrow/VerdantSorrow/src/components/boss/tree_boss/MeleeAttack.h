#pragma once
#include "../../../ecs/Component.h"
#include "../../../utils/Vector2D.h"
#include "../../RectangleCollider.h"
#include "../../../game/CollisionManager.h"
#include <SDL.h>

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

    /**
    Método que permite cambiar la variable active, que indica si el collider se encuentra activo
    *
    @param set: Valor que tomará active
    */
    Entity* getEntity();

protected:
    Transform* tr_;

    TreeMovement* treeMovement_;
    //Variables que controlan el timing del ataque
    int attackDuration;
    int attackCoolDown;
    int lastAttack;

    CollisionManager* colMan_;
    /**
    * Setea la posicion del ataque delante del jugador, teniendo en cuenta su direccion de movimiento
    */
    void setPosition();
};

