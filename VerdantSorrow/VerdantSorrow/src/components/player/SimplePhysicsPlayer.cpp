#include "SimplePhysicsPlayer.h"
#include "../boss/BossAtributos.h"
#include "../../ecs/Entity.h"
#include "../Transform.h"
#include "../boss/frog_boss/wave/WaveMovement.h"
#include "../boss/frog_boss/TongueAttack.h"
#include "../boss/finalBoss/ClapAttack.h"


SimplePhysicsPlayer::SimplePhysicsPlayer(CollisionManager* colManager) : tr_(nullptr), colMan_(colManager), collider_(nullptr), invulnerable_(false), invTimer(0),
exitCollision(false), lastCollision(3, false)
{
}

SimplePhysicsPlayer::~SimplePhysicsPlayer()
{
}

void SimplePhysicsPlayer::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	collider_ = ent_->getComponent<RectangleCollider>();
	attrib_ = ent_->getComponent<PlayerAttributes>();
	gravity_ = ent_->getComponent<SimpleGravity>();
	ctrl_ = ent_->getComponent<PlayerCtrl>();

	assert(tr_ != nullptr && collider_ != nullptr && attrib_ != nullptr);
}

void SimplePhysicsPlayer::update()
{
	//Gravedad
	if (!attrib_->isOnGround() && !gravity_->isActive()) gravity_->setActive(true);
	else if (gravity_->isActive()) gravity_->setActive(false);

	//Colisiones
	if (colMan_->hasCollisions(collider_)) {

		std::vector<RectangleCollider*> colliders = colMan_->getCollisions(collider_);

		for (auto c : colliders) {

			if (c->isActive() && !c->isTrigger()) {

				//colisiones
				auto pos = collider_->getPos(); //jugador
				auto posCollider = c->getPos(); //otros
				auto velCollider = c->getEntity()->getComponent<Transform>()->getVel();

				auto& velPlayer = tr_->getVel();

				float lastPositionX = pos.getX() - velPlayer.getX();
				float lastPositionY = pos.getY() - velPlayer.getY();
				float colliderDiffX = (tr_->getWidth() - collider_->getWidth()) / 2;
				float colliderDiffY = (tr_->getHeight() - collider_->getHeight());

				if (lastPositionX + collider_->getWidth() + velCollider.getX() <= posCollider.getX()) {//colision por la izda
					velPlayer.setX(0);
					tr_->getPos().setX(posCollider.getX() - collider_->getWidth() - colliderDiffX + velCollider.getX());
					attrib_->setRightStop(true);
					lastCollision[1] = true;

				}
				else if (lastPositionX + velCollider.getX() >= posCollider.getX() + c->getWidth()) {//colision por la derecha
					velPlayer.setX(0);
					tr_->getPos().setX(posCollider.getX() + c->getWidth() - colliderDiffX + velCollider.getX());
					attrib_->setLeftStop(true);
					lastCollision[2] = true;
				}
				else if (lastPositionY + collider_->getHeight() + velCollider.getY() <= posCollider.getY()) {//arriba

					velPlayer.setY(0);
					tr_->getPos().setY(c->getPos().getY() - collider_->getHeight() - colliderDiffY + velCollider.getY());
					tr_->getPos().setX(tr_->getPos().getX() + velCollider.getX());
					attrib_->setOnGround(true);
					lastCollision[0] = true;
				}
				else if (lastPositionY>= posCollider.getY() + c->getHeight()) {//abajo
					velPlayer.setY(0);
					tr_->getPos().setY(c->getPos().getY() + c->getHeight() - colliderDiffY);
				}

				exitCollision = true;
			}
			else if (c->isActive() && c->isTrigger()) {

				onCollisionExit();

				ecs::Entity* ent = c->getEntity();
				BossAtributos* bA = ent->getComponent<BossAtributos>();
				ClapAttack* cA = ent->getComponent<ClapAttack>();

				if (bA != nullptr || cA != nullptr) {

					if (!invulnerable_ && !ctrl_->isRolling()) {
						attrib_->damagePlayer(1);
						invulnerable_ = true;
						invTimer = sdlutils().currRealTime();


						// Knock back
						float enemyXpos = ent->getComponent<Transform>()->getPos().getX();
						// Calcular la direccion en la que se realizara el knockback
						int knockbackDir; // Almacenarla en "knockbackDir"
						enemyXpos > tr_->getPos().getX()
							? knockbackDir = 1 : knockbackDir = -1;
						// Informar al controlador
						ctrl_->doKnockback(knockbackDir);
					}

				}

				WaveMovement* wave = ent->getComponent<WaveMovement>();
				if (wave != nullptr) {
					if (!invulnerable_) {
						attrib_->damagePlayer(1);
						invulnerable_ = true;
						invTimer = sdlutils().currRealTime();
					}
				}

			}
		}

		if (invTimer + 5000 > sdlutils().currRealTime()) return;
		invulnerable_ = false;
	}
	else
		onCollisionExit();
}

void SimplePhysicsPlayer::onCollisionExit()
{
	if (exitCollision) {
		if (lastCollision[0]) {
			attrib_->setOnGround(false);
			lastCollision[0] = false;
		}
		if (lastCollision[1]) {
			attrib_->setRightStop(false);
			lastCollision[1] = false;
		}
		if (lastCollision[2]) {
			attrib_->setLeftStop(false);
			lastCollision[2] = false;
		}
		exitCollision = false;

	}
}


