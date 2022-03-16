#include "FinalBossMovement.h"
#include "../../../ecs/Entity.h"
#include "../../../ecs/Manager.h"
#include "../../Transform.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../boss/BossAtributos.h"
#include "HandsManager.h"
#include "../../Image.h"
#include "../frog_boss/wave/WaveMovement.h"
#include "../../../game/CollisionManager.h"

FinalBossMovement::FinalBossMovement(CollisionManager* colManager) :
	tr_(nullptr), colManager_(colManager),  bA_(nullptr), handMngr_(nullptr), phase_(PHASE1), eyeState_(BOUNCE), eyeSpeed(6)
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
	assert(tr_ != nullptr, bA_ != nullptr, handMngr_ != nullptr);
}

void FinalBossMovement::update()
{
	if (phase_ == PHASE1) {
		if (bA_->getLife() <= bA_->getMaxHp() / 2)
			phase_ = PHASE2;

	}
	else if (phase_ == PHASE2) {
		if (eyeState_ == EyeState::BOUNCE) bounce();
		else restartBouncing();
	}
}

void FinalBossMovement::bounce()
{
	auto& pos_ = tr_->getPos();
	auto& vel_ = tr_->getVel();

	if (vel_.magnitude() == 0)
		vel_ = Vector2D(1,1);


	// bounce on top/bottom borders
	if (pos_.getY() < 0) {
		pos_.setY(0.0f);
		vel_.setY(-vel_.getY());
		//sdlutils().soundEffects().at("wall_hit").play();
	}
	else if (pos_.getY() + tr_->getHeight() > sdlutils().height()) {
		pos_.setY(sdlutils().height() - tr_->getHeight());
		
		//Guarda la velocidad y para el objeto actual
		velocitySaved = Vector2D(vel_.getX(), -vel_.getY());
		vel_ = Vector2D(0, 0);
		//Inicia el contador
		lastTimeGround = sdlutils().currRealTime();
		//Crea las dos bolas de fuego
		createWave(1);
		createWave(-1);
		//Cambia el estado a suelo
		eyeState_ = EyeState::GROUND;
		//sdlutils().soundEffects().at("wall_hit").play();
	}
	else if (pos_.getX() + tr_->getWidth() > sdlutils().width()) {
		pos_.setX(sdlutils().width() - tr_->getHeight());
		vel_.setX(-vel_.getX());
		//sdlutils().soundEffects().at("wall_hit").play();
	}
	else if (pos_.getX() < 0) {
		pos_.setX(0);
		vel_.setX(-vel_.getX());
	}
	if(vel_.getX() != 0 && vel_.getY() != 0)
	vel_ = vel_.normalize() * eyeSpeed;
}

void FinalBossMovement::restartBouncing() {
	if (sdlutils().currRealTime() > lastTimeGround + timeInGround) {
		eyeState_ = EyeState::BOUNCE;
		tr_->getVel().set(velocitySaved);
	}
}

void FinalBossMovement::createWave(int dir)
{
	//Se crea la onda expansiva
	auto Wave = mngr_->addEntity();
	//Se anyade el transform
	auto WaveTr = Wave->addComponent<Transform>();
	auto WaveX = tr_->getPos().getX();
	auto WaveY = sdlutils().height() - 80;
	//dir = {-1, 1}
	auto WaveDir = dir;
	if (WaveDir == 1) {
		WaveX += tr_->getWidth();
	}
	auto WaveSpeed = 5;
	//Se le dan las posiciones iniciales, velocidad, ancho y alto a la onda
	WaveTr->init(Vector2D(WaveX, WaveY), Vector2D(), 90, 80, 0.0f);
	//Se le anyade un color inicial a la onda
	Wave->addComponent<Image>(&sdlutils().images().at("bolaFuego"));

	//Se anyade un collider a la onda
	auto waveCollider = Wave->addComponent<RectangleCollider>(WaveTr->getWidth(), WaveTr->getHeight());
	waveCollider->setIsTrigger(true);
	//Se anyade el collider al colliderGameManager
	colManager_->addCollider(waveCollider);
	//Se anyade el movimiento horizontal
	Wave->addComponent<WaveMovement>(WaveDir, WaveSpeed);
}


