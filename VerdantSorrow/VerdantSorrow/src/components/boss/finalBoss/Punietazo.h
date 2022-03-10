#pragma once
#include "../../../ecs/Component.h"
#include "../../../game/CollisionManager.h"
using ecs::Manager;
class Entity;
class CollisionManager;
class Punietazo :
    public ecs::Component
{
public:
    __CMPID_DECL__(ecs::_PUNIETAZO)
     Punietazo();
    ~Punietazo();
    void initComponent()override;
    void godown();
    void persiguejugador();
    void punietazo();
    void makesamall();
    void update()override;

private:
    enum State {
        DOWN = 0, // just before starting a new game
        FOLLOW, // between rounds
        PUNIETAZO, // playing
        FUERA // game over
    };
    State state_;
    float time;
    Transform* tr_;
    CollisionManager* colmanager_;
    RectangleCollider* rect_;
    Transform* playertr_;
    float speed;
    bool para = false;
    float tiempo2;
    bool stop=false;
    float initialwidth_;
    float initialheight_;
};

