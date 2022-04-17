#include "Mushroom.h"
#include "../boss/BossAtributos.h"
#include "../../ecs/Entity.h"
#include "../Transform.h"
#include "../boss/wave/WaveMovement.h"
#include "../boss/frog_boss/TongueAttack.h"
#include "../boss/finalBoss/ClapAttack.h"


Mushroom::Mushroom(CollisionManager* colManager) : tr_(nullptr), colMan_(colManager), collider_(nullptr)
{
}

Mushroom::~Mushroom()
{
}

void Mushroom::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	collider_ = ent_->getComponent<RectangleCollider>();

	assert(tr_ != nullptr && collider_ != nullptr);
	collider_->setIsTrigger(true);
}

void Mushroom::update()
{
	//Colisiones
	if (colMan_->hasCollisions(collider_)) {

		for (auto c : colMan_->getCollisions(collider_)) {

			if (c->isActive() && !c->isTrigger()) {

				//colisiones
				auto playerPos = c->getEntity()->getComponent<RectangleCollider>()->getPos(); //jugador
				auto mushroomPos = collider_->getPos();
				auto& velPlayer = c->getEntity()->getComponent<Transform>()->getVel();
				auto& velCollider = tr_->getVel();

				float lastPositionX = playerPos.getX() - velPlayer.getX();
				float lastPositionY = playerPos.getY() - velPlayer.getY();
				float colliderDiffX = (tr_->getWidth() - collider_->getWidth()) / 2;
				float colliderDiffY = (tr_->getHeight() - collider_->getHeight()) / 2;
				if (lastPositionY + c->getEntity()->getComponent<RectangleCollider>()->getHeight() <= mushroomPos.getY()) {//arriba
					velPlayer.setY(velPlayer.getY() * -1);
				}
				else if (lastPositionX + c->getEntity()->getComponent<RectangleCollider>()->getWidth() <= mushroomPos.getX()) {//colision por la izda
					velPlayer.setX(velPlayer.getX() * -1);
				}
				else if (lastPositionX >= mushroomPos.getX() + c->getWidth()) {//colision por la derecha
					velPlayer.setX(velPlayer.getX() * -1);
				}
				else if (lastPositionY >= mushroomPos.getY() + c->getHeight()) {//abajo
					velPlayer.setY(velPlayer.getY() * -1);
				}
			}
		}
	}
}


