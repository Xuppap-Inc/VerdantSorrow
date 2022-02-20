#include "Attack.h"
#include "../../ecs/Entity.h"
#include "../Transform.h"
#include "../../sdlutils/InputHandler.h"
#include "../../sdlutils/SDLUtils.h"
#include "../player/PlayerCtrl.h"

Attack::Attack(float width, float height): tr_(nullptr), RectangleCollider(width, height), attackDuration(500),attackCoolDown(1000)
{
	setActive(false);
}

Attack::~Attack()
{
}

void Attack::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);

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
