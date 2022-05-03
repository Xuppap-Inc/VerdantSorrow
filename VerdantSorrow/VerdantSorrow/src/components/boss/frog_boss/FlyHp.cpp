#include "FlyHp.h"
#include "FrogAttackManager.h"
#include "../../../ecs/Entity.h"
#include "../../../game/SceneManager.h"
#include "../../../game/Hub.h"

FlyHp::FlyHp() : fAM_()
{
}

FlyHp::FlyHp(FrogAttackManager* fAM):fAM_(fAM)
{
	SoundEffect* s = &sdlutils().soundEffects().at("sfx_mosca_idle");
	s->play();
	s->setChannelVolume(*sC().getHubScene()->getMusicVolume());
}

FlyHp::~FlyHp()
{
	
}

// Metodo que mata a la mosca y notifica al FrogAttackManager
void FlyHp::receiveHit()
{
	fAM_->onFlyDied();
	ent_->setAlive(false);
}
