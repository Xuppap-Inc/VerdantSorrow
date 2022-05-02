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
#include "../../fondos/ParticleSystem.h"

HammerArm::HammerArm(CollisionManager* colManager) :colManager_(colManager), tr_(nullptr), state_(REPOSO), initialPos(), waveSp_(),
lastTimeFloor_(), collider_(), playerTr_(), playerXPos()
{
}

HammerArm::~HammerArm()
{
}

void HammerArm::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	collider_ = ent_->getComponent<RectangleCollider>();
	playerTr_ = mngr_->getHandler(ecs::_PLAYER)->getComponent<Transform>();
	waveSp_ = mngr_->getHandler(ecs::_WAVE_GENERATOR)->getComponent<WaveSpawner>();
	assert(tr_ != nullptr, collider_ != nullptr, playerTr_ != nullptr, waveSp_ != nullptr);

	initialPos = Vector2D(tr_->getPos().getX(), tr_->getPos().getY());
	lastTimeFloor_ = mngr_->addTimer();

}

void HammerArm::goDiagonal()
{
	collider_->setActive(false);

	if (abs(tr_->getPos().getX() - playerXPos) > 5 || abs(tr_->getPos().getY()) > 5) {
		Vector2D dir = Vector2D(playerXPos - tr_->getPos().getX(), -tr_->getPos().getY());
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
			ParticleSystem* particlesys = new ParticleSystem(&sdlutils().images().at("luz_morado"), mngr_);
			particlesys->createParticlesSmash(50, tr_->getPos().getX() + (tr_->getWidth() / 2), tr_->getPos().getY() + tr_->getHeight());
		}

		ParticleSystem* particlesys = new ParticleSystem(&sdlutils().images().at("particula_tierra"), mngr_);
		particlesys->createParticlesSmash(40, tr_->getPos().getX() + (tr_->getWidth() / 2), tr_->getPos().getY() + tr_->getHeight());

		tr_->getVel().set(Vector2D(0, 0));
		tr_->getPos().setY(sdlutils().height() - tr_->getHeight() - 50);
		lastTimeFloor_->reset();
		changeState(REPOSOSUELO);
		waveSp_->createWaves(75, 50, Vector2D(1, 0), tr_);
	}
}

void HammerArm::goBack()
{
	if (abs(tr_->getPos().getX() - initialPos.getX()) > handSpeed + 1 || abs(tr_->getPos().getY() - initialPos.getY()) > handSpeed + 1) {

		Vector2D dir = initialPos - tr_->getPos();
		tr_->getVel().set(dir.normalize() * handSpeed);
	}
	else {

		tr_->getVel().set(Vector2D(0, 0));
		tr_->getPos().set(initialPos);
		changeState(FIN);
	}
}

void HammerArm::stayFloor()
{
	collider_->setIsTrigger(false);
	if (lastTimeFloor_->currTime() > cooldoownInFloor)
		changeState(BACK);
}

void HammerArm::getPlayerX()
{
	playerXPos = playerTr_->getPos().getX();
}
