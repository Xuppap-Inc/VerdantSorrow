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
#include "../../../sdlutils/VirtualTimer.h"
#include "../../fondos/ParticleSystem.h"
#include "../../../game/Hub.h"

FinalBossMovement::FinalBossMovement(CollisionManager* colManager) :
	tr_(nullptr), colManager_(colManager), bA_(nullptr), phase_(PHASE1), eyeState_(BOUNCE),
	eyeSpeed_(3), waveSp_(), lastTimeInGround_(), anim_(), ashes_(), deadBoss_(false), musica_(),
	playerTr_(), musicVolume_(60)
{
}

FinalBossMovement::~FinalBossMovement()
{
}

void FinalBossMovement::initComponent()
{
	auto volume = sC().getHubScene()->getMusicVolume();
	musicVolume_ = *volume;
	tr_ = ent_->getComponent<Transform>();
	bA_ = ent_->getComponent<BossAtributos>();
	anim_ = ent_->getComponent<FramedImage>();
	waveSp_ = mngr_->getHandler(ecs::_WAVE_GENERATOR)->getComponent<WaveSpawner>();
	playerTr_ = mngr_->getHandler(ecs::_PLAYER)->getComponent<Transform>();
	assert(tr_ != nullptr, bA_ != nullptr, waveSp_ != nullptr, playerTr_ != nullptr);

	lastTimeInGround_ = mngr_->addTimer();


	musica_ = &sdlutils().musics().at("music_manos");
	musica_->play();
	musica_->setMusicVolume(musicVolume_);

	SoundEffect* s = &sdlutils().soundEffects().at("sfx_manos_enter");
	s->play();
	s->setChannelVolume(musicVolume_);

	ashes_ = new ParticleSystem(&sdlutils().images().at("particle"), mngr_);
	ashes_->createParticlesAsh(100);

	ParticleSystem* particlesys = new ParticleSystem(&sdlutils().images().at("esencia_etheria"), mngr_);
	particlesys->createParticlesBossSign(1);
	ParticleSystem* particlesys2 = new ParticleSystem(&sdlutils().images().at("luz_rosa"), mngr_);
	particlesys2->createParticlesBossSpawn(100, tr_->getPos().getX() + (tr_->getWidth() / 2), tr_->getPos().getY() + (tr_->getHeight() / 2));
	ParticleSystem* particlesys3 = new ParticleSystem(&sdlutils().images().at("particula_damage"), mngr_);
	particlesys3->createParticlesBossSpawn(100, tr_->getPos().getX() + (tr_->getWidth() / 2), tr_->getPos().getY() + (tr_->getHeight() / 2));
	ParticleSystem* particlesys4 = new ParticleSystem(&sdlutils().images().at("luz_morado"), mngr_);
	particlesys4->createParticlesBossSpawn(100, tr_->getPos().getX() + (tr_->getWidth() / 2), tr_->getPos().getY() + (tr_->getHeight() / 2));

}

void FinalBossMovement::update()
{
	if (active_) {
		
		if (phase_ == PHASE1) {

			checkPhaseChange();
		}
		else if (phase_ == PHASE2) {

			if (eyeState_ == EyeState::BOUNCE) bounce();
			else restartBouncing();
		}
	}

	else if (returningToCenter) {
	
		//vector cuya magnitud es la distancia desde la cabeza al centro de la pantalla
		auto distance = Vector2D(sdlutils().width() / 2, sdlutils().height() / 2) - 
			Vector2D(tr_->getPos().getX() + tr_->getWidth() / 2, tr_->getPos().getY() + tr_->getHeight() / 2);

		if (distance.magnitude() <= 2 * RETURNING_SPEED) {
		
			tr_->getVel().set(0, 0);
			returningToCenter = false;
		}
		else {
		
			distance = distance.normalize();

			tr_->getVel().set(distance * RETURNING_SPEED);
		}
	}

	if (deadBoss_) {

		deadBoss_ = false;
	}
}

void FinalBossMovement::checkPhaseChange()
{
	if (bA_->getLife() <= bA_->getMaxHp() / 2) {

		anim_->changeanim(&sdlutils().images().at("FinalBoss_Fase2"), 5, 4, 800, 20, "FinalBoss_Fase2");
		phase_ = PHASE2;
		tr_->setScale(.33);
		SoundEffect* s = &sdlutils().soundEffects().at("sfx_manos_quemado");
		s->play();
		s->setChannelVolume(musicVolume_);
		SoundEffect* s2 = &sdlutils().soundEffects().at("sfx_manos_damage");
		s2->play();
		s2->setChannelVolume(musicVolume_);

		ashes_->targetParticles(tr_);

		ParticleSystem* particlesys = new ParticleSystem(&sdlutils().images().at("luz_rosa"), mngr_);
		particlesys->createParticlesDandellion(20);

		ParticleSystem* particlesys2 = new ParticleSystem(&sdlutils().images().at("particula_simbolo2_frente"), mngr_);
		particlesys2->createBackgroundParticlesSymbols(6);
	}
}

//este metodo seria mejor separarlo en una clase auxiliar que herede de OnBorders,
//pero como se realizan muchas comprobaciones para hacer avisos de ataques y pararse
//seria bastante complejo hacer esta separacion sin romper el funcionamiento
void FinalBossMovement::bounce()
{
	auto& pos_ = tr_->getPos();
	auto& vel_ = tr_->getVel();

	if (vel_.magnitude() == 0)
		vel_ = Vector2D(1, 1);

	//Aviso ataques
	if ((pos_.getY() < 150 && vel_.getY() < 0) || (pos_.getY() > sdlutils().height() - 300 && vel_.getY() > 0)) {

		anim_->setColor(200, 200, 20, 100);
	}

	// bounce on top/bottom borders
	// Bola de fuego al tocar el techo
	if (pos_.getY() < 0) {

		pos_.setY(0.0f);
		vel_.setY(-vel_.getY());
		fireBall();
	}
	else if (pos_.getY() + tr_->getHeight() > sdlutils().height() - 150) {

		pos_.setY(sdlutils().height() - tr_->getHeight() - 150);

		//Guarda la velocidad y para el objeto actual
		velocitySaved = Vector2D(vel_.getX(), -vel_.getY());
		vel_ = Vector2D(0, 0);

		//Inicia el contador
		lastTimeInGround_->reset();

		//Crea las dos bolas de fuego
		waveSp_->createWaves(100, 100, Vector2D(1, 0), tr_, &sdlutils().images().at("wave"));

		//Cambia el estado a suelo
		eyeState_ = EyeState::GROUND;
	}
	else if (pos_.getX() + tr_->getWidth() > sdlutils().width()) {

		pos_.setX(sdlutils().width() - tr_->getWidth());
		vel_.setX(-vel_.getX());
	}
	else if (pos_.getX() < 0) {

		pos_.setX(0);
		vel_.setX(-vel_.getX());
	}

	if (vel_.magnitude() != 0)
		vel_ = vel_.normalize() * eyeSpeed_;
}

void FinalBossMovement::restartBouncing() {

	if (lastTimeInGround_->currTime() > + WAIT_ON_GROUND) {

		eyeState_ = EyeState::BOUNCE;
		tr_->getVel().set(velocitySaved);
	}
}

void FinalBossMovement::fireBall()
{
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
	waveImgEntTr_->init(Vector2D(waveX - 115, waveY), Vector2D(), width, height, 0.0f, .4f);
	waveImgEntTr_->setScale(8);
	waveImgEnt->addComponent<FramedImage>(&sdlutils().images().at("vfx_manos_fuego"), 6, 6, (1000 / 30) * 30, 30, "vfx_manos_fuego");
	waveImgEnt->addComponent<WaveMovement>(Vector2D(0, 1), fireballSpeed, false);

	SoundEffect* s = &sdlutils().soundEffects().at("sfx_manos_fuego");
	s->play();
	s->setChannelVolume(musicVolume_);

	auto waveCollider = wave->addComponent<RectangleCollider>(width - 20, height - 20);
	waveCollider->setIsTrigger(true);
	colManager_->addCollider(waveCollider);

	waveImgEnt->addToGroup(ecs::_BOSSELEMENTS_GRP);
	wave->addComponent<WaveMovement>(Vector2D(0, 1), fireballSpeed);
}

void FinalBossMovement::setDeadBoss(bool set)
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
