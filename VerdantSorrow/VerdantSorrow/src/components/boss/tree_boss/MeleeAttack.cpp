#include "MeleeAttack.h"
#include "../../../ecs/Entity.h"
#include "../../Transform.h"
#include "../../../sdlutils/InputHandler.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../boss/tree_boss/TreeMovement.h"
#include "../../player/PlayerAttributes.h"
#include "../../../ecs/Manager.h"

MeleeAttack::MeleeAttack(float width, float height, CollisionManager* colManager) : tr_(nullptr), RectangleCollider(width, height), attackDuration(800), attackCoolDown(800), lastAttack(), attacking_(false)
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

		if (currentTime >= lastAttack + attackDuration) {
			setActive(false);
			treeMovement_->setMoveActive(true);
			attacking_ = false;
		}
	}
}


void MeleeAttack::render()
{
	if (isActive()) {
		SDL_SetRenderDrawColor(sdlutils().renderer(), 0, 0, 0, 255);

		SDL_Rect dest = getCollider();

		auto sW = mngr_->getWindowScaleWidth();
		auto sH = mngr_->getWindowScaleHeight();

		dest.x *= sW;
		dest.w *= sW;
		dest.y *= sH;
		dest.h *= sH;

		SDL_RenderFillRect(sdlutils().renderer(), &dest);
	}
}

void MeleeAttack::attack(int dir)
{
	auto currentTime = sdlutils().currRealTime();
	
	//a�ade el propio collider porque meleeAttack ya es un RectangleCollider
	colMan_->addCollider(this);
	setActive(true);
	setIsTrigger(true);
	lastAttack = currentTime;
	setPosition(dir);
	std::cout << "attack called" << std::endl;

	attacking_ = true;
}

void MeleeAttack::setPosition(int dir)
{
	Vector2D pos = tr_->getPos();

	if (dir > 0) {
		pos_ = Vector2D(pos.getX() + tr_->getWidth()/1.5, pos.getY());		
	}
		
	else {
		pos_ = Vector2D(pos.getX() + tr_->getWidth() / 6, pos.getY());
	}		

	treeMovement_->setMoveActive(false);
}
