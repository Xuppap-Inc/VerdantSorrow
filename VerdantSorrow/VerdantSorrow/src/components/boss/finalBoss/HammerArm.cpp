#include "HammerArm.h"
#include "../../../ecs/Entity.h"
#include "../../Transform.h"
#include "../../RectangleCollider.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../../ecs/Manager.h"
#include "../wave/WaveMovement.h"
#include "../../RectangleRenderer.h"
#include "../../../game/CollisionManager.h"
#include "../wave/WaveSpawner.h"
#include "../BossAtributos.h"

HammerArm::HammerArm(CollisionManager* colManager) :colManager_(colManager), tr_(nullptr), state_(REPOSO), initialPos(), waveSp_()
{
}

HammerArm::~HammerArm()
{
}

void HammerArm::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	collider_ = ent_->getComponent<RectangleCollider>();
	playertr_ = mngr_->getHandler(ecs::_PLAYER)->getComponent<Transform>();
	waveSp_ = mngr_->getHandler(ecs::_WAVE_GENERATOR)->getComponent<WaveSpawner>();
	assert(tr_ != nullptr, collider_ != nullptr, playertr_ != nullptr, waveSp_ != nullptr);

	initialPos = Vector2D(tr_->getPos().getX(), tr_->getPos().getY());
}

void HammerArm::goDiagonal()
{
	collider_->setActive(false);

	if (abs(tr_->getPos().getX() - playerXPos) > 5 || abs(tr_->getPos().getY()) > 5) {
		Vector2D dir = Vector2D(playerXPos - tr_->getPos().getX(), -tr_->getPos().getY());/*initialPos - tr_->getPos();*/
		tr_->getVel().set(dir.normalize() * handSpeed);
	}
	else {
		tr_->getVel().set(Vector2D(0, 0));
		tr_->getPos().set(Vector2D(playerXPos, 0));
		changeState(HIT);
	}
}

void HammerArm::attack(bool quemado)
{
	collider_->setActive(true);
	collider_->setIsTrigger(true);

	if (tr_->getPos().getY() < sdlutils().height() - tr_->getHeight() - 50) {
		tr_->getVel().set(Vector2D(0, handSpeed * 4));
	}
	else {
		SoundEffect* s = &sdlutils().soundEffects().at("sfx_manos_attack");
		s->play();
		if (quemado)
		{
			SoundEffect* s2 = &sdlutils().soundEffects().at("sfx_manos_quemado");
			s2->play();
		}
		
		tr_->getVel().set(Vector2D(0, 0));
		tr_->getPos().setY(sdlutils().height() - tr_->getHeight() - 50);
		lastTimeFloor = sdlutils().currRealTime();
		changeState(REPOSOSUELO);
		waveSp_->createWaves(75, 50, Vector2D(1, 0), tr_);
	}
}

void HammerArm::goBack()
{
	if (abs(tr_->getPos().getX() - initialPos.getX()) > 5 || abs(tr_->getPos().getY() - initialPos.getY()) > 5) {
		Vector2D dir = initialPos - tr_->getPos();
		tr_->getVel().set(dir.normalize() * handSpeed);
	}
	else {
		tr_->getVel().set(Vector2D(0, 0));
		tr_->getPos().set(initialPos);
		changeState(FIN);
	}
}

void HammerArm::stayFloor() {
	collider_->setIsTrigger(false);
	if (sdlutils().currRealTime() > lastTimeFloor + cooldoownInFloor)
		changeState(BACK);
}

void HammerArm::getPlayerX()
{
	playerXPos = playertr_->getPos().getX();
}
