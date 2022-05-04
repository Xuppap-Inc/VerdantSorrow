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
#include "../components/Image.h"
#include "../components/boss/tree_boss/MeleeAttack.h"
#include "../components/fondos/ParticleSystem.h"
#include "../components/CameraShake.h"
#include "Hub.h"
#include "../components/tutorial/TutorialRootMovement.h"
#include "TutorialScene.h"



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
			MeleeAttack* mA = ent->getComponent<MeleeAttack>();
			LanternMovement* lantern = ent->getComponent<LanternMovement>();

			if (sC().getScene() == SceneManager::scenes::Tutorial_) {
				auto player = mngr_->getHandler(ecs::_PLAYER);

				//Si es el tutorial y una raíz te devuelve al principio
				if(!player->getComponent<PlayerCtrl>()->isRolling() && c->getEntity()->getComponent<TutorialRootMovement>())
					player->getComponent<Transform>()->getPos().setX(100);

				//Si tiene componente image que solo tiene la puerta cambia de escena
				else if (c->getEntity()->getComponent<Image>() && !c->getEntity()->getComponent<TutorialRootMovement>()) {
					sC().changeStatePlayerInBoss(false);
					sC().getTutorialScene()->changeSceneState(true);
					sC().changeScene(SceneManager::Hub_);
				}
			}
			else if ((bA != nullptr && sC().getScene() != SceneManager::scenes::Eye_) && lantern == nullptr || wave != nullptr || tA != nullptr || cA != nullptr||mA!=nullptr)
				hurtPlayerAndKnockback(player, ent);
		}
	}
	if (vt_->currTime() > attrib->getInvulnerableTimer())
	{
		attrib->setInvulnerable(false);
	}

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

				BossAtributos* bA = nullptr;
				Transform* bTr = nullptr;

				if (SceneManager::scenes::Frog_ == sC().getScene()) {
					bA = mngr_->getHandler(ecs::_FROGBOSS)->getComponent<BossAtributos>();
					bTr = mngr_->getHandler(ecs::_FROGBOSS)->getComponent<Transform>();
				}
				else if (SceneManager::scenes::Tree_ == sC().getScene()){
					bA = mngr_->getHandler(ecs::_LANTERN)->getComponent<BossAtributos>(); 
					bTr = mngr_->getHandler(ecs::_LANTERN)->getComponent<Transform>();
				}
				else if (SceneManager::scenes::Eye_ == sC().getScene()){
					bA = mngr_->getHandler(ecs::_EYE)->getComponent<BossAtributos>(); 
					bTr = mngr_->getHandler(ecs::_EYE)->getComponent<Transform>();
				}

				if (bA != nullptr && bA == ent->getComponent<BossAtributos>()) {

					Transform* playerTr = player->getComponent<Transform>();

					if (playerAt->isNewAttack()) {

						player->getComponent<Attack>()->attackCollided();

						auto camShake = mngr_->getHandler(ecs::_hdlr_CAMERA)->getComponent<CameraShake>();
						if (camShake != nullptr) {

							camShake->shake(5, 5);
						}

						SoundEffect* s = &sdlutils().soundEffects().at("sfx_chica_attack1");
						s->setChannelVolume(*sC().getHubScene()->getMusicVolume());
						s->play();
						s->setChannelVolume(*sC().getHubScene()->getMusicVolume());

						bA->setDamage(PLAYER_ATTACK_DMG);

						FramedImage* bFImg = ent->getComponent<FramedImage>();
						if (bFImg != nullptr) {
							bFImg->setColor(200, 50, 50, 500);
						}
						else
						{
							Image* bImg = ent->getComponent<Image>();
							if (bImg != nullptr) {
								bImg->setColor(200, 50, 50, 500);
							}
						}

						playerAt->setNewAttack(false);
						playerAt->setFinished(false);
					}
				}
				FlyHp* fHP = ent->getComponent<FlyHp>();
				if (fHP != nullptr)
					fHP->receiveHit();

				TutorialFly* tFl = ent->getComponent<TutorialFly>();
				if (tFl != nullptr) {
					mngr_->getHandler(ecs::_hdlr_TUTORIALENTRY)->getComponent<Image>()->setVisible(true);
					mngr_->getHandler(ecs::_hdlr_TUTORIALENTRY)->getComponent<RectangleCollider>()->setActive(true);
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
	FramedImage* playerFImg = player->getComponent<FramedImage>();


	if (!attrib->getInvulnerable() && !playerCtrl->isRolling()) {
		attrib->damagePlayer(1);
		attrib->setInvulnerable(true);
		attrib->setInvulnerableTimer(5000);

		ParticleSystem* particlesys = new ParticleSystem(&sdlutils().images().at("particula_damage"), mngr_);
		particlesys->createParticlesDamagePlayer(50, playerTr->getPos().getX() + (playerTr->getWidth() / 2), playerTr->getPos().getY() + (playerTr->getHeight() / 2));

		SoundEffect* s = &sdlutils().soundEffects().at("sfx_player_damage");
		s->play();
		s->setChannelVolume(*sC().getHubScene()->getMusicVolume());

		playerFImg->setColor(200, 50, 50, 1000);

		// Knock back
		float enemyXpos = ent->getComponent<Transform>()->getPos().getX() + ent->getComponent<Transform>()->getWidth() / 2;
		// Calcular la direccion en la que se realizara el knockback
		// Informar al controlador
		playerCtrl->doKnockback(enemyXpos >= (playerTr->getPos().getX() + playerTr->getWidth() / 2) ? -1 : 1);
		vt_->reset();

		auto camShake = mngr_->getHandler(ecs::_hdlr_CAMERA)->getComponent<CameraShake>();
		if (camShake != nullptr) {
			camShake->shake(10, 10);
		}
	}
}
