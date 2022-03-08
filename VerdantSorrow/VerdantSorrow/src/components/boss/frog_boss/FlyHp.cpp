#include "FlyHp.h"
#include "FrogAttackManager.h"
#include "../../../ecs/Entity.h"

FlyHp::FlyHp() : fAM_()
{
}

FlyHp::FlyHp(FrogAttackManager* fAM):fAM_(fAM)
{
}

FlyHp::~FlyHp()
{
	
}

void FlyHp::receiveHit()
{
	fAM_->onFlyDied();
	ent_->setAlive(false);
	//std::cout << "Golpiado" << std::endl;
}
