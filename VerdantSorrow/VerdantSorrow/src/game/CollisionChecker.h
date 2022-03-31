#pragma once
#include "../ecs/Manager.h"
using ecs::Entity;
class CollisionManager;
class Attack;
class CollisionChecker
{
public:
	CollisionChecker(CollisionManager* colManager, ecs::Manager* mngr);
	virtual ~CollisionChecker();
	void checkCollisions();
private:
	void checkAttackCollisions(Attack* playerAt, ecs::Entity* player);
	void hurtPlayerAndKnockback(ecs::Entity* player, ecs::Entity* ent);
	CollisionManager* colManager_;
	ecs::Manager* mngr_;

	const int PLAYER_ATTACK_DMG = 1;
};

