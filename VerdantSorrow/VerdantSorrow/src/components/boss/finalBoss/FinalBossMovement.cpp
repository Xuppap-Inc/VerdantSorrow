#include "FinalBossMovement.h"
#include "../../../ecs/Entity.h"
#include "../../../ecs/Manager.h"
#include "../../Transform.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../boss/BossAtributos.h"
#include "HandsManager.h"
#include "../../Image.h"
#include "../wave/WaveMovement.h"
#include "../../../game/CollisionManager.h"
#include "../wave/WaveSpawner.h"
#include "../../FramedImage.h"

FinalBossMovement::FinalBossMovement(CollisionManager* colManager) :
	tr_(nullptr), colManager_(colManager), bA_(nullptr), handMngr_(nullptr), phase_(PHASE1), eyeState_(BOUNCE), eyeSpeed(3), waveSp_(), fireBallCooldown_(), lastFireBall_()
{
}

FinalBossMovement::~FinalBossMovement()
{
}

void FinalBossMovement::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	bA_ = ent_->getComponent<BossAtributos>();
	handMngr_ = ent_->getComponent<HandsManager>();
	anim_ = ent_->getComponent<FramedImage>();
	waveSp_ = mngr_->getHandler(ecs::_WAVE_GENERATOR)->getComponent<WaveSpawner>();
	playerTr = mngr_->getHandler(ecs::_PLAYER)->getComponent<Transform>();
	assert(tr_ != nullptr, bA_ != nullptr, handMngr_ != nullptr, waveSp_ != nullptr, playerTr != nullptr);
}

void FinalBossMovement::update()
{
	if (phase_ == PHASE1) {
		if (bA_->getLife() <= bA_->getMaxHp() / 2) {
			anim_->changeanim(&sdlutils().images().at("FinalBoss_Fase2"), 5, 4, 800, 20, "FinalBoss_Fase2");
			phase_ = PHASE2;
			tr_->setScale(.33);
			SoundEffect* s = &sdlutils().soundEffects().at("sfx_manos_quemado");
			s->play();
			SoundEffect* s2 = &sdlutils().soundEffects().at("sfx_manos_damage");
			s2->play();
		}

		lastFireBall_ = sdlutils().currRealTime();
		fireBallCooldown_ = 2000;
	}
	else if (phase_ == PHASE2) {
		if (eyeState_ == EyeState::BOUNCE) bounce();
		else restartBouncing();

		//if (sdlutils().currRealTime() > lastFireBall_ + fireBallCooldown_)
		//	fireBall();
	}
}

void FinalBossMovement::bounce()
{
	auto& pos_ = tr_->getPos();
	auto& vel_ = tr_->getVel();

	if (vel_.magnitude() == 0)
		vel_ = Vector2D(1, 1);


	// bounce on top/bottom borders
	if (pos_.getY() < 0) {
		pos_.setY(0.0f);
		vel_.setY(-vel_.getY());
	}
	else if (pos_.getY() + tr_->getHeight() > sdlutils().height()) {
		pos_.setY(sdlutils().height() - tr_->getHeight());

		//Guarda la velocidad y para el objeto actual
		velocitySaved = Vector2D(vel_.getX(), -vel_.getY());
		vel_ = Vector2D(0, 0);
		//Inicia el contador
		lastTimeGround = sdlutils().currRealTime();
		//Crea las dos bolas de fuego
		waveSp_->createWaves(100, 100, Vector2D(1, 0), tr_, &sdlutils().images().at("bolaFuego"));
		//Cambia el estado a suelo
		eyeState_ = EyeState::GROUND;
		//sdlutils().soundEffects().at("wall_hit").play();
	}
	else if (pos_.getX() + tr_->getWidth() > sdlutils().width()) {
		pos_.setX(sdlutils().width() - tr_->getWidth());
		vel_.setX(-vel_.getX());
		//sdlutils().soundEffects().at("wall_hit").play();
	}
	else if (pos_.getX() < 0) {
		pos_.setX(0);
		vel_.setX(-vel_.getX());
	}
	// Bola de fuego al tocar el techo
	else if (pos_.getY() <= 1) {
		fireBall();
	}

	if (vel_.magnitude() != 0)
		vel_ = vel_.normalize() * eyeSpeed;
}

void FinalBossMovement::restartBouncing() {
	if (sdlutils().currRealTime() > lastTimeGround + timeInGround) {
		eyeState_ = EyeState::BOUNCE;
		tr_->getVel().set(velocitySaved);
	}
}

void FinalBossMovement::fireBall()
{
	////waveSp_->createWave(50, 50, playerTr->getPos() - tr_->getPos(), tr_, &sdlutils().images().at("bolaFuego"));
	//waveSp_->createWave(50, 50, Vector2D(0, 8), tr_, &sdlutils().images().at("bolaFuego"));

	//fireBallCooldown_ = sdlutils().rand().nextInt(1000, 5001);
	//lastFireBall_ = sdlutils().currRealTime();

	int height = 120;
	int width = 100;
	int fireballSpeed = 5;

	auto wave = mngr_->addEntity();

	float waveX, waveY = tr_->getPos().getY() + tr_->getHeight() - height;
	waveX = tr_->getPos().getX() + tr_->getWidth() / 2;

	auto tr_ = wave->addComponent<Transform>();
	tr_->init(Vector2D(waveX, waveY), Vector2D(), width, height, 0.0f, .4f);
	

	auto waveImgEnt = mngr_->addEntity();
	auto waveImgEntTr_ = waveImgEnt->addComponent<Transform>();
	waveImgEntTr_->init(Vector2D(waveX-115, waveY), Vector2D(), width, height, 0.0f, .4f);
	waveImgEntTr_->setScale(8);
	waveImgEnt->addComponent<FramedImage>(&sdlutils().images().at("vfx_manos_fuego"), 6, 6, (1000 / 30) * 30, 30, "vfx_manos_fuego");
	waveImgEnt->addComponent<WaveMovement>(Vector2D(0, 1), fireballSpeed);

	SoundEffect* s = &sdlutils().soundEffects().at("sfx_manos_fuego");
	s->play();

	auto waveCollider = wave->addComponent<RectangleCollider>(width - 20, height - 20);
	waveCollider->setIsTrigger(true);
	colManager_->addCollider(waveCollider);

	wave->addComponent<WaveMovement>(Vector2D(0, 1), fireballSpeed);
}
