#pragma once
#include "../../../ecs/Component.h"
#include "../../../game/CollisionManager.h"
using ecs::Manager;
class Entity;
class Tranform;
class CollisionManager;
class HandsManager :
    public ecs:: Component
{
public:
    __CMPID_DECL__(ecs::_HANDS_MANAGER)
    HandsManager(CollisionManager* col_);
    ~HandsManager();
    void initComponent()override;

private:
    ecs::Entity* leftHand_;
    ecs::Entity* rightHand_;
    Transform* tr_;
    CollisionManager* colmanager_;
};

