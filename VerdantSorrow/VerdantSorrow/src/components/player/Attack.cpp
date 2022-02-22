#include "Attack.h"
#include "../../ecs/Entity.h"
#include "../Transform.h"
#include "../../sdlutils/InputHandler.h"
#include "../../sdlutils/SDLUtils.h"
#include "../player/PlayerCtrl.h"
#include "../FrogBoss/BossAtributos.h"

Attack::Attack(float width, float height, CollisionManager* colManager): tr_(nullptr), RectangleCollider(width, height), attackDuration(500),attackCoolDown(1000)
{
	setActive(false);
	colMan_ = colManager;
	invTimer = 0;
	invulnerable_ = true;
}

Attack::~Attack()
{
}

void Attack::initComponent()
{
	collider_ = this;

	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr, collider_ !=nullptr);

	lastAttack = sdlutils().currRealTime();
}

void Attack::update()
{
	auto& ihdlr = ih();
	auto currentTime = sdlutils().currRealTime();

	if (isActive()) { //si esta activo, se coloca en la posicion correspondiente
		
		setPosition();

		if (currentTime >= lastAttack + attackDuration)
			setActive(false);	
	}
	else {
		if (ihdlr.keyDownEvent()) {//si no esta activo, comprueba si se puede activar (cooldown y j presioada)
			if (ihdlr.isKeyDown(SDLK_j)) {
				if (currentTime >= lastAttack + attackDuration + attackCoolDown) {
					setActive(true);
					lastAttack = currentTime;

					setPosition();//si no setamos la posicion aqui, se renderizara un frame del ataque en una posicion que no debe
				}
			}
		}
	}

	//Colisiones
	if (colMan_->hasCollisions(collider_)) {

		std::vector<RectangleCollider*> colliders = colMan_->getCollisions(collider_);

		for (auto c : colliders) {

			if (c->isActive() && c->isTrigger()) {
				ecs::Entity* ent = c->getEntity();
				BossAtributos* bA = ent->getComponent<BossAtributos>();
				if (bA != nullptr) {
					if (!invulnerable_) {
						bA->setDamage(1);
						invulnerable_ = true;
						invTimer = sdlutils().currRealTime();
					}
				}
			}
		}

		if (invTimer + 1000 > sdlutils().currRealTime()) return;
		invulnerable_ = false;
	}
}

void Attack::render()
{
	if(isActive()){
		Texture* tx = &sdlutils().images().at("star");
		tx->render(getCollider());
	}
}

Entity* Attack::getEntity() {
	return ent_;
}

void Attack::setPosition()
{
	int playerMovementDir = ent_->getComponent<PlayerCtrl>()->getMovementDir();

	Vector2D contPos = tr_->getPos();

	if (playerMovementDir >= 0)
		pos_ = Vector2D(contPos.getX() + tr_->getWidth(), contPos.getY());
	else 
		pos_ = Vector2D(contPos.getX() - width_, contPos.getY());
}
