#include "MeleeAttack.h"
#include "../../../ecs/Entity.h"
#include "../../Transform.h"
#include "../../../sdlutils/InputHandler.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../boss/tree_boss/TreeMovement.h"
#include "../../player/PlayerAttributes.h"

MeleeAttack::MeleeAttack(float width, float height, CollisionManager* colManager) : tr_(nullptr), RectangleCollider(width, height), attackDuration(200), attackCoolDown(300), lastAttack()
{
	setActive(false);
	colMan_ = colManager;
}

MeleeAttack::~MeleeAttack()
{
}

void MeleeAttack::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	treeMovement_ = ent_->getComponent<TreeMovement>();
	assert(tr_ != nullptr, collider_ != nullptr, treeMovement_ !=  nullptr);
}

void MeleeAttack::update()
{
	auto currentTime = sdlutils().currRealTime();

	if (isActive()) { //si esta activo, se coloca en la posicion correspondiente

		setPosition();

		if (currentTime >= lastAttack + attackDuration)
			setActive(false);
	}
	else {	
		if (treeMovement_->isNextToPlayer() && currentTime >= lastAttack + attackDuration + attackCoolDown) {
			setActive(true);
			lastAttack = currentTime;
			setPosition();//si no setamos la posicion aqui, se renderizara un frame del ataque en una posicion que no debe
		}
	}
}


void MeleeAttack::render()
{
	if (isActive()) {
		SDL_SetRenderDrawColor(sdlutils().renderer(), 0, 0, 0, 255);

		SDL_Rect dest = getCollider();

		SDL_RenderFillRect(sdlutils().renderer(), &dest);

	}
}

void MeleeAttack::setPosition()
{
	int grootMovementDir = ent_->getComponent<TreeMovement>()->getMovementDir();

	Vector2D contPos = tr_->getPos();

	if (grootMovementDir >= 0)
		pos_ = Vector2D(contPos.getX() + tr_->getWidth(), contPos.getY());
	else
		pos_ = Vector2D(contPos.getX() - width_, contPos.getY());
}
