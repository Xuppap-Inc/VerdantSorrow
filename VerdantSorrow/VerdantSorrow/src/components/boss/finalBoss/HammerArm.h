#pragma once
#include "../../../ecs/Component.h"
#include "../../../utils/Vector2D.h"
#include "../../RectangleCollider.h"
#include "../../../game/CollisionManager.h"
#include <SDL.h>

class Transform;
class CollisionManager;
class WaveSpawner;

class HammerArm : public ecs::Component
{
public:

    enum State {
        DIAGONAL = 0,
        HIT,
        BACK,
        REPOSO,
        REPOSOSUELO,
        FIN
    };

    __CMPID_DECL__(ecs::_HAMMER_ARM)

        HammerArm(CollisionManager* colManager);
        ~HammerArm();

    void initComponent() override;

    void attack();
    void goBack();
    void goDiagonal();
    void stayFloor();
    void getPlayerX();

    State getstate() { return state_; };
    void changeState(State s) { state_ = s; };

protected:
    Transform* tr_;
    RectangleCollider* collider_;
    int handSpeed = 6, cooldoownInFloor = 1500, lastTimeFloor;
    Transform* playertr_;
    int playerXPos;
    CollisionManager* colManager_;

    Vector2D initialPos;
    State state_;

    WaveSpawner* waveSp_;
};

