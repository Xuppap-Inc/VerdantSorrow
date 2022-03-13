#pragma once
#include "../../../ecs/Component.h"
#include "../../../game/CollisionManager.h"
using ecs::Manager;
class Entity;
class Tranform;
class CollisionManager;
class Punietazo;
class HandsManager :
    public ecs:: Component
{
public:
    __CMPID_DECL__(ecs::_HANDS_MANAGER)
    HandsManager(CollisionManager* col_);
    ~HandsManager();
    void initComponent()override;
    void update();

private:
    ecs::Entity* leftHand_;
    ecs::Entity* rightHand_;
    Transform* tr_;
    CollisionManager* colmanager_;
    Punietazo* punietazoright_;
    Punietazo* punietazoleft_;
};

