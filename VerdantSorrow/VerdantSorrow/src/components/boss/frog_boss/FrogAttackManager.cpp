#include "FrogAttackManager.h"
#include "../../../ecs/Entity.h"
#include "../../../ecs/Manager.h"
#include "FrogBigJump.h"
#include "FrogJump.h"
#include "TongueAttack.h"
#include "../../Transform.h"
#include "../wave/WaveMovement.h"
#include "../../RectangleCollider.h"
#include "../../../game/CollisionManager.h"
#include "../BossAtributos.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../FramedImage.h"
#include "FlyHp.h"
#include "../wave/WaveSpawner.h"
#include "FlyMovement.h"
#include "../../Image.h"
#include "../../fondos/ParticleSystem.h"

FrogAttackManager::FrogAttackManager(CollisionManager* collManager) : frogJump_(), bigJump_(),
fly_(), player_(), tr_(), collManager_(collManager), frogState_(START_ANIMATION), attr_(),
jumping_(false), jumpingBig_(false), jumpDirection_(-1), jumpsUntilNextTongue_(3), delay_(0), musicaFase1_(), musicaFase2_(),
flySpacing_(0), animState_(ANIM_IDLE), tongue_(), attacking_(false), secondPhase_(false), 
animNewState_(ANIM_IDLE), waveSp_(), anim_(), tongueAnim_(), oldJumpDirection_(0), deadBoss_(false), deathTimer_(), startTimer_()
{
}

FrogAttackManager::~FrogAttackManager()
{
}

void FrogAttackManager::initComponent()
{

	frogJump_ = ent_->addComponent<FrogJump>(17);
	bigJump_ = ent_->addComponent<FrogBigJump>(22);

	tr_ = ent_->getComponent<Transform>();
	player_ = mngr_->getHandler(ecs::_PLAYER)->getComponent<Transform>();
	attr_ = ent_->getComponent<BossAtributos>();
	anim_ = ent_->getComponent<FramedImage>();
	waveSp_ = mngr_->getHandler(ecs::_WAVE_GENERATOR)->getComponent<WaveSpawner>();

	vt_ = mngr_->addTimer();
	startTimer_ = mngr_->addTimer();
	deathTimer_ = mngr_->addTimer();

	//musica
	musicaFase2_ = &sdlutils().musics().at("musica_rana_fase2");
	musicaFase2_->play();
	musicaFase2_->setMusicVolume(0);

	musicaFase1_ = &sdlutils().soundEffects().at("musica_rana_fase1");
	musicaFase1_->play(10, 0);
	musicaFase1_->setChannelVolume(60, 0);
	
	dandellions1_ = new ParticleSystem(&sdlutils().images().at("particula_dandellion"), mngr_);
	dandellions1_->createParticlesDandellion(50);

	dandellions2_ = new ParticleSystem(&sdlutils().images().at("particula_dandellion_frente"), mngr_);
	dandellions2_->createOverlayParticlesDandellion(3);

	//lengua
	createTongue(collManager_);

	bool correct = tr_ != nullptr && frogJump_ != nullptr && bigJump_ != nullptr && player_ != nullptr && waveSp_ != nullptr;
	assert(correct);
}

//Patrones de ataque de la rana
void FrogAttackManager::update()
{
	if (!deadBoss_) checkIfDead();

	checkJumpDirection();

	checkPhaseChange();

	checkFrogState();
	
	checkAnimationState();
}

// Si el estado de animacion ha cambiado, cambia la animacion actual a la correspondiente, creando eventos de animacion cuando sean necesarios
void FrogAttackManager::checkAnimationState()
{
	if (animState_ != animNewState_) {
		animState_ = animNewState_;

		//callback para los eventos de animacion
		std::function<void()> callback;

		switch (animState_)
		{
		case FrogAttackManager::ANIM_IDLE:
			anim_->repeat(true);
			if (!secondPhase_)anim_->changeanim(&sdlutils().images().at("rana_idle"), 4, 6, (1000 / 30) * 24, 24, "rana_idle");
			else anim_->changeanim(&sdlutils().images().at("rana_enfadada_idle"), 4, 6, (1000 / 30) * 24, 24, "rana_enfadada_idle");
			break;
		case FrogAttackManager::ANIM_JUMP:
			anim_->setColor(255, 200, 20, 200);
			anim_->repeat(false);

			callback = [this]() { frogState_ = JUMPING; frogJump_->attack(jumpDirection_); };

			if (!secondPhase_) {
				anim_->changeanim(&sdlutils().images().at("rana_jump"), 6, 6, (1000 / 30) * 31, 31, "rana_jump");

				anim_->registerEvent(std::pair<int, std::string>(10, "rana_jump"), callback);
			}
			else {
				anim_->changeanim(&sdlutils().images().at("rana_enfadada_jump"), 6, 6, (1000 / 30) * 31, 31, "rana_enfadada_jump");

				anim_->registerEvent(std::pair<int, std::string>(10, "rana_enfadada_jump"), callback);
			}
			break;
		case FrogAttackManager::ANIM_BIG_JUMP:
			anim_->setColor(255, 200, 20, 500);
			anim_->repeat(false);

			callback = [this]() { frogState_ = JUMPING_BIG; bigJump_->attack(jumpDirection_); };

			if (!secondPhase_) {
				anim_->changeanim(&sdlutils().images().at("rana_jump"), 6, 6, (1000 / 30) * 31, 31, "rana_jump");
				anim_->registerEvent(std::pair<int, std::string>(10, "rana_jump"), callback);
			}
			else {
				anim_->changeanim(&sdlutils().images().at("rana_enfadada_jump"), 6, 6, (1000 / 30) * 31, 31, "rana_enfadada_jump");
				anim_->registerEvent(std::pair<int, std::string>(10, "rana_enfadada_jump"), callback);
			}
			break;
		case FrogAttackManager::ANIM_TONGUE:
			anim_->setColor(255, 200, 20, 500);
			anim_->repeat(false);

			//callback del ataque de la lengua
			callback = [this]() { frogState_ = CASTING_TONGUE; };

			if (!secondPhase_) {

				anim_->changeanim(&sdlutils().images().at("rana_lengua"), 4, 6, (1000 / 30) * 24, 24, "rana_lengua");
				anim_->registerEvent(std::pair<int, std::string>(23, "rana_lengua"), callback);
			}

			else {

				anim_->changeanim(&sdlutils().images().at("rana_enfadada_lengua"), 4, 6, (1000 / 30) * 24, 24, "rana_enfadada_lengua");
				anim_->registerEvent(std::pair<int, std::string>(23, "rana_enfadada_lengua"), callback);
			}

			if (anim_->getFrameNum() == 24) anim_->select_sprite(6, 4);
			break;
		case FrogAttackManager::ANIM_CHANGE_PHASE:
			anim_->repeat(false);

			//callback que cambia el estado a waiting
			callback = [this]() { frogState_ = WAITING; animNewState_ = ANIM_IDLE; };

			anim_->changeanim(&sdlutils().images().at("rana_cambio_fase"), 4, 6, (1000 / 30) * 23, 23, "rana_cambio_fase");

			anim_->registerEvent(std::pair<int, std::string>(22, "rana_cambio_fase"), callback);
			break;
		case FrogAttackManager::ANIM_JUMP_TO_VULNERABLE:
			anim_->repeat(false);
			anim_->changeanim(&sdlutils().images().at("rana_salto_a_vulnerable"), 2, 6, (1000 / 30) * 10, 10, "rana_salto_a_vulnerable");
			callback = [this]() { animNewState_ = ANIM_VULNERABLE; };
			anim_->registerEvent(std::pair<int, std::string>(9, "rana_salto_a_vulnerable"), callback);
			break;
		case FrogAttackManager::ANIM_VULNERABLE:
			anim_->repeat(false);
			anim_->changeanim(&sdlutils().images().at("rana_vulnerable"), 4, 6, (1000 / 30) * 21, 21, "rana_vulnerable");
			callback = [this]() { animNewState_ = ANIM_VULNERABLE_TO_IDLE; };
			anim_->registerEvent(std::pair<int, std::string>(20, "rana_vulnerable"), callback);
			break;
		case FrogAttackManager::ANIM_VULNERABLE_TO_IDLE:
			anim_->repeat(false);
			anim_->changeanim(&sdlutils().images().at("rana_vulnerable_a_idle"), 1, 5, (1000 / 30) * 5, 5, "rana_vulnerable_a_idle");
			callback = [this]() { animNewState_ = ANIM_IDLE; frogState_ = CALC_NEXT_ATTACK; };
			anim_->registerEvent(std::pair<int, std::string>(4, "rana_vulnerable_a_idle"), callback);
			break;
		case FrogAttackManager::ANIM_DEATH:
			anim_->repeat(false);
			anim_->changeanim(&sdlutils().images().at("rana_enfadada_muerte"), 3, 6, (1000 / 30) * 16, 16, "rana_enfadada_muerte");
			break;
		default:
			break;
		}
	}
}

// Comprueba el estado actual de la rana y realiza las acciones correspondientes de cada uno
void FrogAttackManager::checkFrogState()
{
	auto& rand = sdlutils().rand();

	std::function<void()> callback;

	switch (frogState_) {
	case JUMPING:
		if (attr_->isOnGround()) {
			onGrounded(jumping_, false);
		}
		break;
	case JUMPING_BIG:
		if (attr_->isOnGround()) {
			onGrounded(jumpingBig_, true);
		}
		break;
	case TONGUE:
		if (tongue_->getComponent<TongueAttack>()->finished())
		{
			frogState_ = DOING_ANIMATION;

			callback = [this] {
				frogState_ = WAITING;

				tongueAnim_->setVisible(false);

				tongue_->setActive(false);

				delay_ = 0;
				vt_->reset();
			};

			//animacion de recoger la lengua
			tongueAnim_->changeanim(&sdlutils().images().at("lenguaRecoger"), 2, 2, (1000 / 20) * 4, 4, "lenguaRecoger");

			tongueAnim_->registerEvent(std::pair<int, std::string>(3, "lenguaRecoger"), callback);
		}
		break;
	case CALC_NEXT_ATTACK:
		nextAttack();
		break;
	case WAITING:
		
		if (delay_< vt_->currTime()) {
			frogState_ = CALC_NEXT_ATTACK;
		}
		break;
	case CASTING_TONGUE:

		callback = [this] {
			tongue_->getComponent<TongueAttack>()->attack(!secondPhase_);
			frogState_ = TONGUE;
		};

		frogState_ = DOING_ANIMATION;

		tongue_->getComponent<TongueAttack>()->currentPos(!secondPhase_);

		tongueAnim_->changeanim(&sdlutils().images().at("lengua"), 2, 2, (1000 / 20) * 4, 4, "lengua");
		tongueAnim_->repeat(false);
		tongueAnim_->adjustToTransform(true);

		if (jumpDirection_ == 1) tongueAnim_->flipX(true);
		else tongueAnim_->flipX(false);

		tongueAnim_->registerEvent(std::pair<int, std::string>(3, "lengua"), callback);
		tongueAnim_->setVisible(true);

		break;
	case FLY_DIED:
		if (!jumping_ && !jumpingBig_) {

			oldJumpDirection_ = jumpDirection_;
			jumpDirection_ = 0;

			tongue_->getComponent<TongueAttack>()->cancel();
			tongue_->setActive(false);
			
			jumpingBig_ = true;

			frogState_ = DOING_ANIMATION;
			//Lanzar animacion de salto largo
			animNewState_ = ANIM_BIG_JUMP;
		}
		else
		{
			if (jumping_) frogState_ = JUMPING;
			else if (jumpingBig_) frogState_ = JUMPING_BIG;
		}
		break;
	case START_ANIMATION:
		if (startTimer_->currTime() >= START_DELAY) 
		{
			frogState_ = WAITING;
			startTimer_->pause();
		}
		break;
	case DYING:
		if (deathTimer_->currTime() >= DEATH_DELAY) {

			attr_->setDefeated(true);
		}
		break;
	default:
		break;
	}
}

void FrogAttackManager::checkIfDead()
{
	if (attr_->getLife() <= 0) {

		frogState_ = DYING;
		animNewState_ = ANIM_DEATH;

		deadBoss_ = true;
		deathTimer_->reset();

		auto col = ent_->getComponent<RectangleCollider>();
		col->setActive(false);

		ParticleSystem* particlesys = new ParticleSystem(&sdlutils().images().at("particula_esencia"), mngr_);
		particlesys->createParticlesEssence(10, tr_->getPos().getX() - tr_->getWidth() / 2, tr_->getPos().getY() + tr_->getHeight() / 2, player_);
	}
}

void FrogAttackManager::checkPositionPlayer()
{
	auto posPlayer = player_->getPos();
	auto frogPos = tr_->getPos();

	auto distance = posPlayer.getX() - frogPos.getX();
	
	if (distance < 0) 
	{
		anim_->flipX(false);
		jumpDirection_ = -1;
	}
	
	else 
	{
		anim_->flipX(true);
		jumpDirection_ = 1;
	}
}

ecs::Entity* FrogAttackManager::createFly()
{
	fly_ = mngr_->addEntity();
	auto fTr = fly_->addComponent<Transform>();

	auto flyY = sdlutils().height() - player_->getHeight() - FLOOR_HEIGHT;
	
	auto flyX = player_->getPos().getX();

	fTr->init(Vector2D(flyX, flyY), Vector2D(), 100, 100, 0.0f);

	//collider
	auto coll = fly_->addComponent<RectangleCollider>(fTr->getWidth(), fTr->getHeight());
	coll->setIsTrigger(true);
	collManager_->addCollider(coll);

	fly_->addComponent<FramedImage>(&sdlutils().images().at("mosca"), 6, 6, 2000, 31, "mosca");
	fly_->addComponent<FlyHp>(this);
	
	mngr_->setHandler(ecs::_FLY, fly_);
	fly_->addToGroup(ecs::_BOSS_GRP);
	return fly_;
}

ecs::Entity* FrogAttackManager::createTongue(CollisionManager* colManager)
{
	tongue_ = mngr_->addEntity();

	auto tr = tongue_->addComponent<Transform>();

	auto tongueY = sdlutils().height() - player_->getHeight() - FLOOR_HEIGHT;
	auto tongueW = 100;
	auto tongueH = 50;
	auto tongueX = tr_->getPos().getX()-tongueW;
	tr->init(Vector2D(tongueX, tongueY), Vector2D(), tongueW, tongueH, 0.0f);

	auto tongueCollider = tongue_->addComponent<TongueAttack>();
	colManager->addCollider(tongueCollider);

	tongueAnim_ = tongue_->addComponent<FramedImage>(&sdlutils().images().at("lengua"), 2, 2, (1000/20)*4, 4, "lengua");
	tongueAnim_->repeat(false);
	tongueAnim_->setVisible(false);
	tongueAnim_->adjustToTransform(true);

	tongue_->addToGroup(ecs::_BOSS_GRP);
	return tongue_;
}

void FrogAttackManager::onFlyDied() {
	frogState_ = FLY_DIED;
}

void FrogAttackManager::deactivateBoss()
{
	frogState_ = DOING_ANIMATION;
	animNewState_ = ANIM_IDLE;
}

void FrogAttackManager::checkJumpDirection()
{
	if (jumpDirection_ == 1 && !anim_->isFlipped()) {
		jumpDirection_ = -1;
	}
	else if (jumpDirection_ == -1 && anim_->isFlipped()) {
		jumpDirection_ = 1;
	}
}

void FrogAttackManager::onGrounded(bool& jump, bool isBig)
{
	frogState_ = WAITING;
	jump = false;
	delay_ = sdlutils().rand().nextInt(1000, 2500);
	vt_->reset();
	if (isBig) {
		if (secondPhase_) {
			jumpsUntilNextTongue_--;
			animNewState_ = ANIM_IDLE;
		}
		else {
			animNewState_ = ANIM_JUMP_TO_VULNERABLE; 
			frogState_ = DOING_ANIMATION;
		}
		waveSp_->createWaves(200, 70, Vector2D(1, 0), tr_);

		if (jumpDirection_ == 0) jumpDirection_ = oldJumpDirection_;
	}
	else {
		jumpsUntilNextTongue_--;
		animNewState_ = ANIM_IDLE;
	}
}

// Metodo que calcula cual debe de ser el siguiente ataque de la rana, teniendo en cuenta el numero de saltos que lleva desde el ultimo ataque de lengua
void FrogAttackManager::nextAttack()
{
	if (jumpsUntilNextTongue_ == 0) {
		jumpsUntilNextTongue_ = sdlutils().rand().nextInt(3, 5);
		
		checkPositionPlayer();

		animNewState_ = ANIM_TONGUE;

		if (!secondPhase_) {
			createFly();
		}
		frogState_ = DOING_ANIMATION;

		tongue_->setActive(true);
	}
	else {
		int nextJump = secondPhase_ ? sdlutils().rand().nextInt(0, 100) : 100;
		if (nextJump >= 30) {
			//lanzar animacion de salto en el metodo attack
			animNewState_ = ANIM_JUMP;
			frogState_ = DOING_ANIMATION;
		}
		else {
			//lanzar animavion de salto en el metodo attack
			frogState_ = DOING_ANIMATION;
			animNewState_ = ANIM_BIG_JUMP;
		}
	}
}

void FrogAttackManager::checkPhaseChange()
{
	if (!secondPhase_ && attr_->getLife() <= attr_->getMaxHp() * 0.5) {

		//Lanzar animacion del cambio de fase
		animNewState_ = ANIM_CHANGE_PHASE;
		SoundEffect* s = &sdlutils().soundEffects().at("sfx_cambio_fase");
		s->play();

		musicaFase2_->setMusicVolume(60);
		musicaFase1_->pauseChannel(0);

		dandellions1_->disolveParticles();
		dandellions2_->disolveParticles();

		ParticleSystem* particlesys = new ParticleSystem(&sdlutils().images().at("particula_lluvia"), mngr_);
		particlesys->createParticlesRain(120);

		frogState_ = DOING_ANIMATION;

		vt_->reset();
		secondPhase_ = true;
	}
}
