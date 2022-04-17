#pragma once
#include "../../../ecs/Component.h"
#include "../../../utils/Vector2D.h"
#include "../../RectangleCollider.h"
#include "../../../game/CollisionManager.h"
#include <SDL.h>

class Transform;
class VirtualTimer;

class ClapAttack : public ecs::Component
{
public:

    enum State {    
        DIAGONAL = 0,
        CENTER,
        BACK,
        REPOSO,
        REPOSOSUELO,
        FIN
    };

    __CMPID_DECL__(ecs::_CLAP_ATTACK)
    
    ClapAttack(bool leftHand);
    ~ClapAttack();

    void initComponent() override;

    void goCenter(bool quemado);
    void goBack();
    void goDiagonal();
    void stayFloor();


    State getstate() { return state_; };
    void changeState(State s) { state_ = s; };

protected:
    Transform* tr_;
    RectangleCollider* collider_;
    bool leftHand_;
    int handSpeed = 6, cooldoownInFloor = 1500;

    VirtualTimer* vt_;


    Vector2D initialPos;
    State state_;
};

