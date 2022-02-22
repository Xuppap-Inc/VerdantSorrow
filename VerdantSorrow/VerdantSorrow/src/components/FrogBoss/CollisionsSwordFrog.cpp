#include "CollisionsSwordFrog.h"
#include "../player/PlayerAttributes.h"
#include "../../ecs/Entity.h"
#include "../Transform.h"
#include "../player/Attack.h"


CollisionsSwordFrog::CollisionsSwordFrog(CollisionManager* colManager, Entity* player) : colMan_(colManager), collider_(nullptr), invulnerable_(false), invTimer(0)
{
	playerEntity_ = player;
}

CollisionsSwordFrog::~CollisionsSwordFrog()
{
}

void CollisionsSwordFrog::initComponent()
{
	collider_ = ent_->getComponent<RectangleCollider>();
	attrib_ = ent_->getComponent<BossAtributos>();
	/*gravity_ = ent_->getComponent<SimpleGravity>();*/
	assert(collider_ != nullptr && attrib_ != nullptr);
}

void CollisionsSwordFrog::update()
{
	//Colisiones
	if (colMan_->hasCollisions(collider_)) {

		Attack* at = playerEntity_->getComponent<Attack>();
		//Compruebo las colisiones de la espada
		std::vector<RectangleCollider*> colliders = colMan_->getCollisions(at);

		for (auto c : colliders) {
			//Si está activa y no es trigger
			if (c->isActive() && c->isTrigger()) {
				ecs::Entity* ent = c->getEntity();
				BossAtributos* bossAt = ent->getComponent<BossAtributos>();
				//Si con o que colisiona es la propia rana la golpea, se pone un tiempo de invulnerabilidad y pierde vida
				if (bossAt != nullptr) {
					if (!invulnerable_) {
						attrib_->setDamage(1);
						invulnerable_ = true;
						std::cout << attrib_->getLife() << std::endl;
						invTimer = sdlutils().currRealTime();
					}
				}
			}
		}

		if (invTimer + 1000 > sdlutils().currRealTime()) return;
		invulnerable_ = false;

	}
}


