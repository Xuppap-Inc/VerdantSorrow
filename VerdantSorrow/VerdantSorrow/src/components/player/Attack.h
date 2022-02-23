#pragma once
#include "../../ecs/Component.h"
#include "../../utils/Vector2D.h"
#include "../RectangleCollider.h"
#include "../../game/CollisionManager.h"
#include <SDL.h>

class Transform;
using ecs::Entity;

class Attack : public RectangleCollider
{
public:
    __CMPID_DECL__(ecs::_ATTACK)
    Attack(float width, float height, CollisionManager* colManager);
    ~Attack();

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

    //Variables que controlan el timing del ataque
    int attackDuration;
    int attackCoolDown;
    int lastAttack;

    Attack* collider_;
    CollisionManager* colMan_;
    bool invulnerable_;
    Uint32 invTimer;
    /**
    * Setea la posicion del ataque delante del jugador, teniendo en cuenta su direccion de movimiento
    */
    void setPosition();
};

