#pragma once
#include "../ecs/Manager.h"
using ecs::Entity;
class CollisionManager;
class CollisionChecker
{
public:
	CollisionChecker(CollisionManager* colManager, ecs::Manager* mngr);
	virtual ~CollisionChecker();
	void collisionsFrogScene();
	void collisionsGrootScene();
	void collisionsFinalBossScene();
private:
	void hurtPlayerAndKnockback(ecs::Entity* player, ecs::Entity* ent);
	CollisionManager* colManager_;
	ecs::Manager* mngr_;
};

