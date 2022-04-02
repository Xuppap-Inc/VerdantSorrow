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
#include "../components/player/Attack.h"
#include "../components/boss/frog_boss/FlyHp.h"
#include "../components/boss/tree_boss/LanternMovement.h"
#include "../components/tutorial/TutorialFly.h"
#include "SceneManager.h"
#include "../components/FramedImage.h"


CollisionChecker::CollisionChecker(CollisionManager* colManager, ecs::Manager* mngr) : colManager_(colManager), mngr_(mngr)
{
}

CollisionChecker::~CollisionChecker()
{
}

void CollisionChecker::checkCollisions()
{
	auto player = mngr_->getHandler(ecs::_PLAYER);
	PlayerAttributes* attrib = player->getComponent<PlayerAttributes>();
	Attack* playerAt = player->getComponent<Attack>();

	for (auto c : colManager_->getCollisions(player->getComponent<RectangleCollider>())) {
		if (c->isActive() && c->isTrigger()) {
			ecs::Entity* ent = c->getEntity();
			BossAtributos* bA = ent->getComponent<BossAtributos>();
			WaveMovement* wave = ent->getComponent<WaveMovement>();
			TongueAttack* tA = ent->getComponent<TongueAttack>();
			ClapAttack* cA = ent->getComponent<ClapAttack>();

			LanternMovement* lantern = ent->getComponent<LanternMovement>();

			if ((bA != nullptr && sC().getScene() != SceneManager::scenes::Eye_) && lantern == nullptr || wave != nullptr || tA != nullptr || cA != nullptr)
				hurtPlayerAndKnockback(player, ent);
		}
	}

	if (sdlutils().currRealTime() > attrib->getInvulnerableTimer() + 5000)
		attrib->setInvulnerable(false);


	checkAttackCollisions(playerAt, player);
}

void CollisionChecker::checkAttackCollisions(Attack* playerAt, ecs::Entity* player)
{
	//Colisiones ataque con boss
	if (playerAt->isActive()) {

		for (auto c : colManager_->getCollisions(playerAt)) {

			if (c->isActive() && c->isTrigger())
			{
				ecs::Entity* ent = c->getEntity();
				BossAtributos* bA = ent->getComponent<BossAtributos>();
				if (bA != nullptr && playerAt->isNewAttack()) {
					SoundEffect* s = &sdlutils().soundEffects().at("sfx_chica_attack1");
					s->setChannelVolume(70);
					s->play();

					/*auto VFXEnt = mngr_->addEntity();
					auto VFXTr = VFXEnt->addComponent<Transform>();
					VFXTr->init(Vector2D(tr_->getPos().getX() - 125, tr_->getPos().getY()), Vector2D(), 400, 200, 0.0f);
					VFXEnt->addComponent<FramedImage>(&sdlutils().images().at("vfx_attack"), 1, 6, (1000 / 30) * 6, 6, "vfx");
					VFXEnt->addComponent<VFX>(6);*/

					auto anim = player->getComponent<FramedImage>();

					//slow de la animacion
					/*std::function<void()> slowAnimCallback = [anim]() { anim->slowAnimation(10, 1); };

					anim->registerEvent(std::pair<int, std::string>(7, "Chica_AtkFloor"), slowAnimCallback);*/

					bA->setDamage(PLAYER_ATTACK_DMG);
					player->getComponent<Transform>()->getVel().setY(0);

					playerAt->setNewAttack(false);
					playerAt->setFinished(false);
				}
				FlyHp* fHP = ent->getComponent<FlyHp>();
				if (fHP != nullptr)
					fHP->receiveHit();

				TutorialFly* tFl = ent->getComponent<TutorialFly>();
				if (tFl != nullptr) {
					tFl->receiveHit();
				}
			}
		}
	}
}

void CollisionChecker::hurtPlayerAndKnockback(ecs::Entity* player, ecs::Entity* ent)
{
	PlayerAttributes* attrib = player->getComponent<PlayerAttributes>();
	PlayerCtrl* playerCtrl = player->getComponent<PlayerCtrl>();
	Transform* playerTr = player->getComponent<Transform>();

	if (!attrib->getInvulnerable() && !playerCtrl->isRolling()) {
		attrib->damagePlayer(1);
		attrib->setInvulnerable(true);
		attrib->setInvulnerableTimer(sdlutils().currRealTime());

		// Knock back
		float enemyXpos = ent->getComponent<Transform>()->getPos().getX() + ent->getComponent<Transform>()->getWidth() / 2;
		// Calcular la direccion en la que se realizara el knockback
		// Informar al controlador
		playerCtrl->doKnockback(enemyXpos >= (playerTr->getPos().getX() + playerTr->getWidth() / 2) ? -1 : 1);
	}
}
