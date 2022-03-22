#include "CollisionChecker.h"
#include "CollisionManager.h"
#include "../components/boss/BossAtributos.h"
#include "../components/boss/finalBoss/ClapAttack.h"
#include "../components/boss/wave/WaveMovement.h"
#include "../components/player/PlayerCtrl.h"
#include "../ecs/Entity.h"
#include "../components/player/SimplePhysicsPlayer.h"
#include "../components/Transform.h"
#include "../components/boss/frog_boss/FrogAttackManager.h"
#include "../components/boss/frog_boss/TongueAttack.h"

CollisionChecker::CollisionChecker(CollisionManager* colManager, ecs::Manager* mngr): colManager_(colManager), mngr_(mngr)
{
}

CollisionChecker::~CollisionChecker()
{
}

void CollisionChecker::collisionsFrogScene()
{
	auto player = mngr_->getHandler(ecs::_PLAYER);
	if (colManager_->hasCollisions(player->getComponent<RectangleCollider>())) {

		std::vector<RectangleCollider*> colliders = colManager_->getCollisions(player->getComponent<RectangleCollider>());

		for (auto c : colliders) {


			if (c->isActive() && c->isTrigger()) {
				player->getComponent<SimplePhysicsPlayer>()->onCollisionExit();

				ecs::Entity* ent = c->getEntity();
				BossAtributos* bA = ent->getComponent<BossAtributos>();
				WaveMovement* wave = ent->getComponent<WaveMovement>();
				TongueAttack* tA = ent->getComponent<TongueAttack>();

				if (bA != nullptr || wave != nullptr || tA != nullptr) {

					if (!player->getComponent<PlayerAttributes>()->getInvulnerable() && !player->getComponent<PlayerCtrl>()->isRolling()) {
						player->getComponent<PlayerAttributes>()->damagePlayer(1);
						player->getComponent<PlayerAttributes>()->setInvulnerable(true);
						player->getComponent<PlayerAttributes>()->setInvulnerableTimer(sdlutils().currRealTime());

						// Knock back
						float enemyXpos = ent->getComponent<Transform>()->getPos().getX() + ent->getComponent<Transform>()->getWidth() / 2;
						// Calcular la direccion en la que se realizara el knockback
						// Informar al controlador
						player->getComponent<PlayerCtrl>()->doKnockback(enemyXpos >= (player->getComponent<Transform>()->getPos().getX() + player->getComponent<Transform>()->getWidth() / 2) ? -1 : 1);
					}

				}
			}
			else //collider no activo
				player->getComponent<SimplePhysicsPlayer>()->onCollisionExit();
		}

		if (player->getComponent<PlayerAttributes>()->getInvulnerableTimer() + 5000 > sdlutils().currRealTime()) return;
		player->getComponent<PlayerAttributes>()->setInvulnerable(false);
	}
}

void CollisionChecker::collisionsGrootScene()
{
	auto player = mngr_->getHandler(ecs::_PLAYER);
	if (colManager_->hasCollisions(player->getComponent<RectangleCollider>())) {

		std::vector<RectangleCollider*> colliders = colManager_->getCollisions(player->getComponent<RectangleCollider>());

		for (auto c : colliders) {


			if (c->isActive() && c->isTrigger()) {
				player->getComponent<SimplePhysicsPlayer>()->onCollisionExit();

				ecs::Entity* ent = c->getEntity();
				BossAtributos* bA = ent->getComponent<BossAtributos>();
				WaveMovement* wave = ent->getComponent<WaveMovement>();

				if (bA != nullptr || wave != nullptr) {

					if (!player->getComponent<PlayerAttributes>()->getInvulnerable() && !player->getComponent<PlayerCtrl>()->isRolling()) {
						player->getComponent<PlayerAttributes>()->damagePlayer(1);
						player->getComponent<PlayerAttributes>()->setInvulnerable(true);
						player->getComponent<PlayerAttributes>()->setInvulnerableTimer(sdlutils().currRealTime());

						// Knock back
						float enemyXpos = ent->getComponent<Transform>()->getPos().getX() + ent->getComponent<Transform>()->getWidth() / 2;
						// Calcular la direccion en la que se realizara el knockback
						// Informar al controlador
						player->getComponent<PlayerCtrl>()->doKnockback(enemyXpos >= (player->getComponent<Transform>()->getPos().getX() + player->getComponent<Transform>()->getWidth() / 2) ? -1 : 1);
					}

				}
			}
			else //collider no activo
				player->getComponent<SimplePhysicsPlayer>()->onCollisionExit();
		}

		if (player->getComponent<PlayerAttributes>()->getInvulnerableTimer() + 5000 > sdlutils().currRealTime()) return;
		player->getComponent<PlayerAttributes>()->setInvulnerable(false);
	}
}
