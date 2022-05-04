#include "EscapeSceneBoss.h"
#include "FinalBossMovement.h"
#include "../../../ecs/Entity.h"
#include "../../../ecs/Manager.h"
#include "../../Transform.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../boss/BossAtributos.h"
#include "../../Image.h"
#include "../../../game/CollisionManager.h"
#include "../../FramedImage.h"
#include "../../../sdlutils/VirtualTimer.h"
#include "../../../game/Hub.h"

EscapeSceneBoss::EscapeSceneBoss(CollisionManager* colManager) :
	tr_(nullptr), colManager_(colManager), bA_(nullptr), eyeState_(BOUNCE),
	eyeSpeed_(3), lastTimeInGround_(), anim_(), ashes_(), deadBoss_(false), musica_(),
	playerTr_(), musicVolume_(60)
{
}
EscapeSceneBoss::~EscapeSceneBoss()
{
}

void EscapeSceneBoss::initComponent()
{

	tr_ = ent_->getComponent<Transform>();
	bA_ = ent_->getComponent<BossAtributos>();
	anim_ = ent_->getComponent<FramedImage>();

	assert(tr_ != nullptr, bA_ != nullptr, waveSp_ != nullptr);

	lastTimeInGround_ = mngr_->addTimer();

}

void EscapeSceneBoss::update()
{
	if (active_) {
		if (eyeState_ == EyeState::BOUNCE) bounce();
		else restartBouncing();
	}
	else if (returningToCenter) {

		//vector cuya magnitud es la distancia desde la cabeza al centro de la pantalla
		auto distance = Vector2D(sdlutils().width() / 2, sdlutils().height() / 2) -
			Vector2D(tr_->getPos().getX() + tr_->getWidth() / 2, tr_->getPos().getY() + tr_->getHeight() / 2);

		if (distance.magnitude() <= 2 * RETURNING_SPEED) {

			returningToCenter = false;
		}
		else {

			distance = distance.normalize();

			tr_->getVel().set(distance * RETURNING_SPEED);
		}
	}

}

void EscapeSceneBoss::bounce()
{
	auto& pos_ = tr_->getPos();
	auto& vel_ = tr_->getVel();

	if (vel_.magnitude() == 0)
		vel_ = Vector2D(1, 1);

	auto cameraTr = mngr_->getHandler(ecs::_hdlr_CAMERA)->getComponent<Transform>();
	//tr_->getVel().set(cameraTr->getPos().getX(), cameraTr->getPos().getY());
	// bounce on top/bottom borders
	if (pos_.getY() < cameraTr->getPos().getY() + cameraTr->getHeight() / 2) {

		pos_.setY(cameraTr->getPos().getY() + cameraTr->getHeight() / 2);
		vel_.setY(-vel_.getY());
	}
	else if (pos_.getY() + tr_->getHeight() > cameraTr->getPos().getY() + sdlutils().height() - 100) {

		pos_.setY(cameraTr->getPos().getY() + sdlutils().height() - tr_->getHeight() - 100);
		vel_.setY(-vel_.getY());

		////Guarda la velocidad y para el objeto actual
		//velocitySaved = Vector2D(vel_.getX(), -vel_.getY());
		//vel_ = Vector2D(0, 0);

		////Inicia el contador
		//lastTimeInGround_->reset();



		////Cambia el estado a suelo
		//eyeState_ = EyeState::GROUND;
	}
	else if (pos_.getX() + tr_->getWidth() > cameraTr->getPos().getX() + cameraTr->getWidth() / 2) {

		pos_.setX(cameraTr->getPos().getX() + cameraTr->getWidth() / 2 - tr_->getWidth());
		vel_.setX(-vel_.getX());
	}
	else if (pos_.getX() < cameraTr->getPos().getX() - cameraTr->getWidth() / 2) {

		pos_.setX(cameraTr->getPos().getX() - cameraTr->getWidth() / 2);
		vel_.setX(-vel_.getX());
	}

	if (vel_.magnitude() != 0)
		vel_ = vel_.normalize() * eyeSpeed_;
}

void EscapeSceneBoss::restartBouncing() {

	if (lastTimeInGround_->currTime() > +WAIT_ON_GROUND) {

		eyeState_ = EyeState::BOUNCE;
		tr_->getVel().set(velocitySaved);
	}
}

void EscapeSceneBoss::setDeadBoss(bool set)
{
	deadBoss_ = set;

	if (set) {
		musica_->pauseMusic();

		SoundEffect* s = &sdlutils().soundEffects().at("sfx_boss_defeated");
		s->play();
		s->setChannelVolume(musicVolume_);
		SoundEffect* s2 = &sdlutils().soundEffects().at("sfx_descubrir_manos");
		s2->play();
		s2->setChannelVolume(musicVolume_);
		SoundEffect* s3 = &sdlutils().soundEffects().at("sfx_avalancha");
		s3->play();
		s3->setChannelVolume(musicVolume_);
	}
}