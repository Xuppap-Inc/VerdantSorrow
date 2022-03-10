#include "ClapAttack.h"
#include "../../../ecs/Entity.h"
#include "../../Transform.h"
#include "../../RectangleCollider.h"
#include "../../../sdlutils/SDLUtils.h"

ClapAttack::ClapAttack(bool leftHand): leftHand_(leftHand), tr_(nullptr)
{
}

ClapAttack::~ClapAttack()
{
}

void ClapAttack::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	collider_ = ent_->getComponent<RectangleCollider>();
	assert(tr_ != nullptr, collider_ != nullptr);
}

void ClapAttack::update()
{
	if (leftHand_) {
		if (!movingToAttack && tr_->getPos().getX() + (tr_->getWidth() / 2) >= sdlutils().width() / 2) {
			collider_->setIsTrigger(false);
			tr_->getVel().set(0, 0);
		}		
		else if (!movingToAttack) {
			tr_->getVel().set(handSpeed / 2, 0);
			collider_->setIsTrigger(true);
		}
			
		//Movimiento de la mano hacia la izquierda y hacia abajo
		else if(tr_->getPos().getX() >= tr_->getWidth() && tr_->getPos().getY() + tr_->getHeight() <= sdlutils().height())
			tr_->getVel().set(-handSpeed, handSpeed);
		else if(tr_->getPos().getX() >= tr_->getWidth()) tr_->getVel().set(-handSpeed, 0);
		else if (tr_->getPos().getY() + tr_->getHeight() <= sdlutils().height()) tr_->getVel().set(0, handSpeed);
		//Si ya está en la esquina izquierda, desactiva el booleano y activa un timer, además de parar el objeto
		else if(movingToAttack) {
			movingToAttack = false;
			tr_->getVel().set(0, 0);
			timerToStart_ = sdlutils().currRealTime();
		}
	}
	else {
		if (!movingToAttack && tr_->getPos().getX() - (tr_->getWidth() / 2) <= sdlutils().width() / 2) {
			tr_->getVel().set(0, 0);
			collider_->setIsTrigger(false);
		}
			
		else if (!movingToAttack){
			tr_->getVel().set(-handSpeed / 2, 0);
			collider_->setIsTrigger(true);
		}
		//Movimiento de la mano hacia la derecha y hacia abajo
		else if (tr_->getPos().getX() + tr_->getHeight() <= sdlutils().width() && tr_->getPos().getY() + tr_->getHeight() <= sdlutils().height())
			tr_->getVel().set(handSpeed, handSpeed);
		else if (tr_->getPos().getX() + tr_->getHeight() <= sdlutils().width()) tr_->getVel().set(handSpeed, 0);
		else if (tr_->getPos().getY() + tr_->getHeight() <= sdlutils().height()) tr_->getVel().set(0, handSpeed);
		//Si ya está en la esquina derecha, desactiva el booleano y activa un timer, además de parar el objeto
		else if(movingToAttack){
			movingToAttack = false;
			tr_->getVel().set(0, 0);
			timerToStart_ = sdlutils().currRealTime();
		}
	}
}
