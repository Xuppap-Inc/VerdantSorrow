#include "ClapAttack.h"
#include "../../../ecs/Entity.h"
#include "../../Transform.h"
#include "../../RectangleCollider.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../../sdlutils/VirtualTimer.h"
#include "../../../ecs/Manager.h"
#include "../../fondos/ParticleSystem.h"
#include "../../../game/Hub.h"
#include "../../CameraShake.h"


ClapAttack::ClapAttack(bool leftHand) : leftHand_(leftHand), tr_(nullptr), state_(REPOSO), vt_(), initialPos(), collider_()
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

	initialPos = Vector2D(tr_->getPos().getX(), tr_->getPos().getY());
	vt_ = mngr_->addTimer();
}

void ClapAttack::goDiagonal()
{
	collider_->setActive(false);
	int xObjective;
	int yOffset = 120;

	if (!leftHand_) xObjective = 0;
	else xObjective = sdlutils().width() - tr_->getWidth();

	if (abs(tr_->getPos().getX() - xObjective) > 5 || abs(tr_->getPos().getY() - sdlutils().height() + tr_->getHeight() + yOffset) > 5) {

		Vector2D dir = Vector2D(xObjective - tr_->getPos().getX(), sdlutils().height() - tr_->getPos().getY() - tr_->getHeight() - yOffset);
		tr_->getVel().set(dir.normalize() * handSpeed);
	}
	else {

		tr_->getVel().set(Vector2D(0, 0));
		tr_->getPos().set(Vector2D(xObjective, sdlutils().height() - tr_->getHeight() - yOffset));
		changeState(CENTER);
	}
}

void ClapAttack::goCenter(bool quemado)
{
	int objectivePos;

	collider_->setActive(true);
	collider_->setIsTrigger(true);


	if (leftHand_) {

		objectivePos = sdlutils().width() / 2;

		if (tr_->getPos().getX() > objectivePos) {

			tr_->getVel().set(Vector2D(-handSpeed * 4, 0));
		}
		else {

			tr_->getVel().set(Vector2D(0, 0));
			tr_->getPos().setX(objectivePos);

			changeState(REPOSOSUELO);

			SoundEffect* s = &sdlutils().soundEffects().at("sfx_manos_attack2");
			s->play();
			s->setChannelVolume(*sC().getHubScene()->getMusicVolume());

			auto camShake = mngr_->getHandler(ecs::_hdlr_CAMERA)->getComponent<CameraShake>();
			camShake->shake(20, 60);

			if (quemado) {
				SoundEffect* s2 = &sdlutils().soundEffects().at("sfx_manos_fire_clap");
				s2->play();

				ParticleSystem* particlesys = new ParticleSystem(&sdlutils().images().at("luz_morado"), mngr_);
				particlesys->createParticlesSmash(20, tr_->getPos().getX(), tr_->getPos().getY() + tr_->getHeight() / 2);
			}

			ParticleSystem* particlesys = new ParticleSystem(&sdlutils().images().at("particula_tierra"), mngr_);
			particlesys->createParticlesSmash(40, tr_->getPos().getX(), tr_->getPos().getY() + tr_->getHeight() / 2);
		}
	}
	else {
		objectivePos = sdlutils().width() / 2 - tr_->getWidth();

		if (tr_->getPos().getX() < objectivePos) {

			tr_->getVel().set(Vector2D(handSpeed * 4, 0));
		}
		else {

			tr_->getVel().set(Vector2D(0, 0));
			tr_->getPos().setX(objectivePos);
			vt_->reset();
			changeState(REPOSOSUELO);
		}
	}
}

void ClapAttack::goBack()
{
	if (abs(tr_->getPos().getX() - initialPos.getX()) > handSpeed+1 || abs(tr_->getPos().getY() - initialPos.getY()) > handSpeed+1) {

		Vector2D dir = initialPos - tr_->getPos();
		tr_->getVel().set(dir.normalize() * handSpeed);
	}
	else {

		tr_->getVel().set(Vector2D(0, 0));
		tr_->getPos().set(initialPos);
		changeState(FIN);
	}
}

void ClapAttack::stayFloor() {

	collider_->setIsTrigger(false);

	if (vt_->currTime() > cooldoownInFloor)
		changeState(BACK);
}
