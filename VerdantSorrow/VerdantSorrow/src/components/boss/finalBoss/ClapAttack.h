#pragma once
#include "../../../ecs/Component.h"
#include "../../../utils/Vector2D.h"
#include "../../RectangleCollider.h"
#include "../../../game/CollisionManager.h"
#include <SDL.h>

class Transform;

class ClapAttack : public ecs::Component
{
public:

    enum State {
        SIDE = 0,
        DOWN,
        CENTER,
        BACK,
        REPOSO,
        FIN
    };

    __CMPID_DECL__(ecs::_CLAP_ATTACK)
    
    ClapAttack(bool leftHand);
    ~ClapAttack();

    void initComponent() override;

    void goSide();
    void goDown();
    void goCenter();
    void goBack();

    State getstate() { return state_; };
    void changeState(State s) { state_ = s; };

protected:
    Transform* tr_;
    RectangleCollider* collider_;
    bool leftHand_;
    int handSpeed = 6;


    Vector2D initialPos;
    State state_;
};

