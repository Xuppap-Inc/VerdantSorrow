#include "SimplePhysicsPlayer.h"
#include "../FrogBoss/BossAtributos.h"
#include "../../ecs/Entity.h"
#include "../Transform.h"


SimplePhysicsPlayer::SimplePhysicsPlayer(CollisionManager* colManager) : tr_(nullptr), colMan_(colManager), collider_(nullptr), invulnerable_(false), invTimer(0)
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

				auto& velPlayer = tr_->getVel();

				float lastPositionX = pos.getX() - velPlayer.getX();
				float lastPositionY = pos.getY() - velPlayer.getY();

				bool leftCollision = lastPositionX + collider_->getWidth() <= posCollider.getX();
				bool rightCollision = lastPositionX >= posCollider.getX() + c->getWidth();
				bool upCollision = lastPositionY + collider_->getHeight() <= posCollider.getY();
				bool downCollision = lastPositionY >= posCollider.getY() + c->getHeight();

				if (upCollision) {//arriba
					if (!attrib_->isOnGround()) {
						velPlayer.setY(0);
						tr_->getPos().setY(c->getPos().getY() - collider_->getHeight());
						attrib_->setOnGround(true);
					}
				}
				else if (downCollision) {//abajo
					velPlayer.setY(0);
					tr_->getPos().setY(c->getPos().getY() + c->getHeight());
				}
				else if (leftCollision) {//colision por la izda
					if (!attrib_->isRightStop()) {
						velPlayer.setX(0);
						tr_->getPos().setX(posCollider.getX() - collider_->getWidth());
						attrib_->setRightStop(true);
					}
				}
				else if (rightCollision) {//colision por la derecha
					if (!attrib_->isLeftStop()) {
						velPlayer.setX(0);
						tr_->getPos().setX(posCollider.getX() + c->getWidth());
						attrib_->setLeftStop(true);
					}
				}

				std::cout << upCollision << " " << leftCollision << " " << downCollision << " " << rightCollision << " " << std::endl;
			}
			else if (c->isActive() && c->isTrigger()) {
				ecs::Entity* ent = c->getEntity();
				BossAtributos* bA = ent->getComponent<BossAtributos>();

				if (bA != nullptr) {

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

}


