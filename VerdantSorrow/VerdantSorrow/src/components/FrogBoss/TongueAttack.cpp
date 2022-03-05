#include "TongueAttack.h"
#include "../Transform.h"
#include "../FramedImage.h"
#include "../../ecs/Entity.h"
#include "../../game/CollisionManager.h"
#include "../../sdlutils/InputHandler.h"
#include "../../ecs/Manager.h"
#include "../RectangleRenderer.h"

TongueAttack::TongueAttack(CollisionManager *colMan) 
{
	RectangleCollider::RectangleCollider();
	setIsTrigger(true);
	setActive(false);
	colMan->addCollider(this);
	
	
}
TongueAttack::TongueAttack(): RectangleCollider()
{
	setIsTrigger(true);
	setActive(false);
}
TongueAttack::~TongueAttack()
{

}
void TongueAttack::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
}
void TongueAttack::update()
{
	auto& ihdlr = ih();
	if (ihdlr.keyDownEvent()) 
	{
		if (ihdlr.isKeyDown(SDL_SCANCODE_U))
		{
			attack(mngr_->getHandler(ecs::_PLAYER)->getComponent<Transform>());
		}
	}
	else if (ihdlr.keyUpEvent()) 
	{

		if (ihdlr.isKeyUp(SDL_SCANCODE_U))
		{
			setActive(false);
		}
	}
}
void TongueAttack::attack(Transform* objective)
{
	
	auto posFrog = tr_->getPos();
	auto posObj = objective->getPos();
	auto iniPos = Vector2D(posFrog.getX(),posFrog.getY());
	auto finPos = Vector2D(posObj.getX(),posObj.getY());
	auto tongW = (finPos - iniPos).magnitude();
	if (posObj.getX() < posFrog.getX()) //Si la mosca esta a su izq, e collider crece hacia la izquierda
	{
		tongW = -tongW;
	}
	setCollider(Vector2D(posFrog.getX(),posFrog.getY()),tongW,50);
	setActive(true);
	
	
}

void TongueAttack::setCollider(Vector2D pos, float w, float h)
{
	pos_ = pos;
	width_ = w;
	height_ = h;

}
