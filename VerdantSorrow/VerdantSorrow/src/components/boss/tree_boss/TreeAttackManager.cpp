#include "TreeAttackManager.h"

#include "../../../ecs/Entity.h"
#include "../../../ecs/Manager.h"
#include "../../Transform.h"
#include "../../RectangleCollider.h"
#include "../../RectangleRenderer.h"
#include "../../../game/CollisionManager.h"
#include "RootWave.h"
#include "RootAutoAim.h"
#include "MeleeAttack.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../FramedImage.h"
#include "../../Image.h"
#include "LanternMovement.h"
#include "TreeMovement.h"
#include "LanternCollisions.h"
#include "../BossAtributos.h"
#include "../../fondos/ParticleSystem.h"
#include "../../../game/Hub.h"

TreeAttackManager::~TreeAttackManager()
{
}

TreeAttackManager::TreeAttackManager(CollisionManager* collManager) : player_(), tr_(), collManager_(collManager), anim_(), 
																	rootWidth_(0), rootAutoAim_(), rootWave_(), meleeAttack_(),
																	attacking_(false), treeCol_(), waiting_(false), 
																	lantern_(), lanternTr_(), lanternMov_(), lanternCols_(), 
																	attribs_(), dir_(0), movement_(), deadBoss_(false), animState_(ANIM_IDLE),
																	animNewState_(ANIM_IDLE), musicVolume_(60)
{
}

void TreeAttackManager::initComponent()
{
	auto volume = sC().getHubScene()->getMusicVolume();
	musicVolume_ = *volume;
	tr_ = ent_->getComponent<Transform>();
	player_ = mngr_->getHandler(ecs::_PLAYER)->getComponent<Transform>();
	treeCol_ = ent_->getComponent<RectangleCollider>();
	movement_ = ent_->getComponent<TreeMovement>();
	movement_->setMoveActive(false);

	anim_ = ent_->getComponent<FramedImage>();

	lantern_ = mngr_->getHandler(ecs::_LANTERN);
	lanternTr_ = lantern_->getComponent<Transform>();
	lanternMov_ = lantern_->getComponent<LanternMovement>();
	lanternCols_ = lantern_->getComponent<LanternCollisions>();
	attribs_ = lantern_->getComponent<BossAtributos>();

	rootWave_ = ent_->getComponent<RootWave>();
	rootAutoAim_ = ent_->getComponent<RootAutoAim>();
	meleeAttack_ = ent_->getComponent<MeleeAttack>();
	
	//Timers
	timerCd_ = mngr_->addTimer();

	timerSpecial_ = mngr_->addTimer();
	timerSpecial_->pause();

	timerWave_ = mngr_->addTimer();
	timerWave_->pause();

	waitTimer_ = mngr_->addTimer();

	startTimer_ = mngr_->addTimer();
	deathTimer_ = mngr_->addTimer();


	//Musica
	musicaFase2_ = &sdlutils().musics().at("musica_linterna_fase2");
	musicaFase2_->play();
	musicaFase2_->setMusicVolume(0);

	musicaFase1_ = &sdlutils().soundEffects().at("musica_linterna_fase1");
	musicaFase1_->play(10, 0);
	musicaFase1_->setChannelVolume(musicVolume_, 0);

	SoundEffect* s = &sdlutils().soundEffects().at("sfx_arbol_enter");
	s->play();
	s->setChannelVolume(musicVolume_);

	hojas_ = new ParticleSystem(&sdlutils().images().at("particula_hoja"), mngr_);
	hojas_->createParticlesDandellion(10);

	deadParticles1_ = new ParticleSystem(&sdlutils().images().at("particula_esencia"), mngr_);
	deadParticles2_ = new ParticleSystem(&sdlutils().images().at("simbolo_lumine"), mngr_);

	ParticleSystem* particlesys = new ParticleSystem(&sdlutils().images().at("esencia_lumine"), mngr_);
	particlesys->createParticlesBossSign(1);
	ParticleSystem* particlesys2 = new ParticleSystem(&sdlutils().images().at("luz_naranja"), mngr_);
	particlesys2->createParticlesBossSpawn(100, tr_->getPos().getX() + (tr_->getWidth() / 2), tr_->getPos().getY() + (tr_->getHeight() / 2));
	ParticleSystem* particlesys3 = new ParticleSystem(&sdlutils().images().at("particula_negro"), mngr_);
	particlesys3->createParticlesBossSpawn(100, tr_->getPos().getX() + (tr_->getWidth() / 2), tr_->getPos().getY() + (tr_->getHeight() / 2));
	ParticleSystem* particlesys4 = new ParticleSystem(&sdlutils().images().at("particula_hoja"), mngr_);
	particlesys4->createParticlesBossSpawn(100, tr_->getPos().getX() + (tr_->getWidth() / 2), tr_->getPos().getY() + (tr_->getHeight() / 2));

	ParticleSystem* particlesys5 = new ParticleSystem(&sdlutils().images().at("spotlight_yellow"), mngr_);
	particlesys5->createLightsFollow(1, lanternTr_, 800);

	bool correct = tr_ != nullptr && player_ != nullptr && rootWave_ != nullptr && rootAutoAim_ != nullptr && meleeAttack_ != nullptr && lanternTr_ != nullptr;
	assert(correct);

	phase = PHASE1;
	state = START_ANIM;

	timerWave_->reset();
	timerSpecial_->reset();
	timerCd_->reset();
	
}

void TreeAttackManager::update()
{
	Vector2D playerPos = player_->getPos();
	Vector2D treePos = tr_->getPos();

	//el arbol es más ancho que su collider, se ajusta jugando con su ancho
	float distance = (playerPos.getX() + player_->getWidth() / 2) - (treePos.getX() + tr_->getWidth() / 2);

	float absDistance = abs(distance);

	checkDirection(distance);

	checkState(absDistance);

	checkAnimState();

	if (!deadBoss_) checkIfDead();
}

void TreeAttackManager::checkIfDead()
{
	if (attribs_->getLife() <= 0) {
		animNewState_ = ANIM_DEATH;
		state = DYING;

		movement_->setMoveActive(false);
		
		lantern_->setActive(false);

		deadBoss_ = true;
		deathTimer_->reset();

		musicaFase2_->pauseMusic();

		SoundEffect* s = &sdlutils().soundEffects().at("sfx_boss_defeated");
		s->play();
		s->setChannelVolume(musicVolume_);
		SoundEffect* s2 = &sdlutils().soundEffects().at("sfx_descubrir_arbol");
		s2->play();
		s2->setChannelVolume(musicVolume_);
		deadParticles1_->createParticlesBossDie(150, tr_->getPos().getX() + (tr_->getWidth() / 2), tr_->getPos().getY() + (tr_->getHeight() / 2));
		deadParticles2_->createParticlesBossDieSymbol(1, tr_->getPos().getX() + (tr_->getWidth() / 2), tr_->getPos().getY() + (tr_->getHeight() / 2));

		auto col = ent_->getComponent<RectangleCollider>();
		col->setActive(false);

		rootAutoAim_->cancelAttack();
	}
}

void TreeAttackManager::checkDirection(float distance)
{
	if (!attacking_) {
		if (distance > 0) dir_ = 1;
		else dir_ = -1;

		//cambio de sentido en las animaciones de groot
		//se hace aquí porque si se hace en el cambio de animaciones
		//hay mucho delay 

		if (dir_ < 0) {
			anim_->flipX(false);
		}
		else anim_->flipX(true);
	}
}

void TreeAttackManager::checkState(float absDistance)
{
	if (state == MOVING) {

		if (meleeAttack_->hasFinished()) attacking_ = false, newAtack_ = true;

		if (!attacking_) {
		
			animNewState_ = ANIM_WALK;
		}

		//si se encuentra a distancia de ataque a melee, ataca
		if ((absDistance < MELEE_ATTACK_DISTANCE) && newAtack_) {

			if (timerCd_->currTime() >= ATTACK_CD) {
				animNewState_ = ANIM_ATTACK;

				SoundEffect* s = &sdlutils().soundEffects().at("sfx_arbol_attack");
				s->play();
				s->setChannelVolume(musicVolume_);

				meleeAttack_->setFinished(false);

				attacking_ = true;
				newAtack_ = false;
				timerCd_->reset();
			}

		}

		if (phase == PHASE1) {

			checkPhaseChange();

			if (timerWave_->currTime() > TIME_BETWEEN_WAVES) attackWave();

			if (timerSpecial_->currTime() > TIME_FOR_SPECIAL) prepareToSpecial();
		}
	}

	else if (state == WAVE) {
		lanternCols_->setDamaged(true); //waves no hacen da�o
		if (rootWave_->getMove()) {

			state = MOVING;

			movement_->setMoveActive(true);

			//timers
			timerWave_->reset();
			timerSpecial_->resume();

			attacking_ = false;
		}
	}

	else if (state == SPECIAL_ATTACK) {

		if (!waiting_ && rootAutoAim_->hasFinished()) {

			//reactiva al arbol
			lanternMov_->setActive(true);
			anim_->fadeIn();

			lanternCols_->setDamaged(true);

			//lo devuelve al centro
			returnToIni();

			waitTimer_->reset();

			waiting_ = true;
		}

		else if (waiting_ && waitTimer_->currTime() > WAIT_AFTER_SPECIAL) {

			state = MOVING;

			//timers
			timerSpecial_->reset();
			timerWave_->resume();

			attacking_ = false;
			movement_->setMoveActive(true);
			lanternMov_->setActive(true);
			treeCol_->setActive(true);

			waitTimer_->reset();
			waitTimer_->pause();

			waiting_ = false;
		}
	}

	else if (state == MOVING_TO_CENTER) {

		if (movement_->hasFinishedMovingToCenter()) {

			animNewState_ = ANIM_BACKGROUND;
			anim_->fadeOut();
			anim_->repeat(false);
		}
	}

	else if (state == DYING)
	{
		if (deathTimer_->currTime() >= DEATH_DELAY) {

			attribs_->setDefeated(true);
		}
	}

	else if (state == START_ANIM) {
	
		if (startTimer_->currTime() >= START_DELAY) {
		
			startTimer_->pause();

			state = MOVING;
			animNewState_ = ANIM_WALK;

			timerWave_->reset();
			timerSpecial_->reset();
			movement_->setMoveActive(true);
		}
	}
}

void TreeAttackManager::attackMelee()
{
	meleeAttack_->attack(dir_);
}

void TreeAttackManager::checkAnimState()
{
	if (animState_ != animNewState_) {
		animState_ = animNewState_;

		std::function<void()> attackCallback;

		switch (animState_)
		{
		case TreeAttackManager::ANIM_IDLE:
			anim_->repeat(true);
			if (phase == PHASE2)anim_->changeanim(&sdlutils().images().at("arbol_idle"), 5, 6, (1000 / 30) * 27, 27, "arbol_idle");
			else anim_->changeanim(&sdlutils().images().at("arbol_capa_idle"), 5, 6, (1000 / 30) * 25, 25, "arbol_capa_idle");
			break;
		case TreeAttackManager::ANIM_WALK:
			anim_->repeat(true);
			if (phase == PHASE2)anim_->changeanim(&sdlutils().images().at("arbol_walk"), 5, 6, (1000 / 30) * 28, 28, "arbol_walk");
			else anim_->changeanim(&sdlutils().images().at("arbol_capa_walk"), 2, 6, (1000 / 30) * 12, 12, "arbol_capa_walk");
			break;
		case TreeAttackManager::ANIM_ATTACK:
			anim_->setColor(255, 200, 20, 200);
			anim_->repeat(false);

			attackCallback = [this]() { attackMelee(); };

			if (phase == PHASE2) {
			
				anim_->registerEvent(std::pair<int, std::string>(16, "arbol_attack"), attackCallback);
				anim_->changeanim(&sdlutils().images().at("arbol_attack"), 4, 6, (1000 / 60) * 24, 24, "arbol_attack");
			}
			else {

				anim_->registerEvent(std::pair<int, std::string>(16, "arbol_capa_attack"), attackCallback);
				anim_->changeanim(&sdlutils().images().at("arbol_capa_attack"), 4, 6, (1000 / 60) * 24, 24, "arbol_capa_attack");
			}
			break;
		case TreeAttackManager::ANIM_ATTACK_COMBO:
			anim_->setColor(255, 200, 20, 200);
			if (phase == PHASE2)anim_->changeanim(&sdlutils().images().at("arbol_attack"), 4, 6, (1000 / 30) * 24, 24, "arbol_attack");
			else anim_->changeanim(&sdlutils().images().at("arbol_capa_attack_combo"), 3, 6, (1000 / 30) * 13, 13, "arbol_capa_attack_combo");
			break;
		case TreeAttackManager::ANIM_ROOTS:
			anim_->repeat(false);
			anim_->setColor(255, 200, 20, 500);

			//callback que llama al ataque de raices al acabar la animacion
			attackCallback = [this]() {
				rootWave_->attack(dir_); state = WAVE; };

			anim_->registerEvent(std::pair<int, std::string>(15, "arbol_capa_roots"), attackCallback);

			anim_->changeanim(&sdlutils().images().at("arbol_capa_roots"), 3, 6, (1000 / 30) * 16, 16, "arbol_capa_roots");
			break;
		case TreeAttackManager::ANIM_BACKGROUND:

			//callback que llama al ataque especial al acabar la animacion
			attackCallback = [this]() { attackSpecial(); };

			anim_->registerEvent(std::pair<int, std::string>(8, "arbol_capa_background"), attackCallback);

			anim_->changeanim(&sdlutils().images().at("arbol_capa_background"), 2, 6, (1000 / 30) * 9, 9, "arbol_capa_background");
			break;
		case TreeAttackManager::ANIM_CHANGE_PHASE:
			anim_->repeat(false);
			anim_->changeanim(&sdlutils().images().at("arbol_capa_cambio_fase"), 3, 6, (1000 / 30) * 14, 14, "arbol_capa_cambio_fase");
			break;
		case TreeAttackManager::ANIM_DEATH:
			anim_->repeat(false);
			anim_->changeanim(&sdlutils().images().at("arbol_muerte"), 3, 6, (1000 / 30) * 16, 16, "arbol_muerte");
			break;
		default:
			break;
		}
	}
}

void TreeAttackManager::deactivateBoss()
{
	state = DOING_ANIM;
	animNewState_ = ANIM_IDLE;

	movement_->setMoveActive(false);
}

bool TreeAttackManager::isSecondPhase()
{
	return attribs_->getLife() <= attribs_->getMaxHp() / 2; 
}

void TreeAttackManager::checkPhaseChange()
{
	if (attribs_->getLife() < attribs_->getMaxHp() / 3) {

		animNewState_ = ANIM_CHANGE_PHASE;
		anim_->repeat(false);

		//callback que llama al cambio de estado al acabar la animacion de cambio de fase
		std::function<void()> changePhaseCallback = [this]() { 
			
			state = MOVING; 
			movement_->setMoveActive(true);
		};
		
		anim_->registerEvent(std::pair<int, std::string>(13, "arbol_capa_cambio_fase"), changePhaseCallback);

		SoundEffect* s = &sdlutils().soundEffects().at("sfx_cambio_fase");
		s->play();
		s->setChannelVolume(musicVolume_);
		SoundEffect* s2 = &sdlutils().soundEffects().at("sfx_wind");
		s2->play();
		s2->setChannelVolume(musicVolume_);
		SoundEffect* s3 = &sdlutils().soundEffects().at("sfx_arbol_damage");
		s3->play();
		s3->setChannelVolume(musicVolume_);

		musicaFase2_->setMusicVolume(musicVolume_);
		musicaFase1_->pauseChannel(0);

		hojas_->disolveParticles();

		ParticleSystem* particlesys = new ParticleSystem(&sdlutils().images().at("particula_hoja"), mngr_);
		particlesys->createParticlesWind(50);

		ParticleSystem* particlesys2 = new ParticleSystem(&sdlutils().images().at("particula_simbolo1_frente"), mngr_);
		particlesys2->createOverlayParticlesDandellion(3);

		phase = PHASE2;

		rootAutoAim_->attack(true);

		lanternMov_->setActive(false);
		lanternMov_->moveToSide();

		lanternCols_->changeToSecondPhase();
		rootAutoAim_->changeToSecondPhase();

		state = CHANGING_PHASE;
	}
}

void TreeAttackManager::returnToIni()
{
	auto treeX = sdlutils().width() / 2 - 80;
	auto treeY = tr_->getPos().getY();
	tr_->getPos().set(Vector2D(treeX, treeY));
}

void TreeAttackManager::attackWave()
{
	if (!attacking_) {

		animNewState_ = ANIM_ROOTS;
		anim_->repeat(false);

		movement_->setMoveActive(false);

		//pausa el timer del otro ataque
		timerSpecial_->pause();

		state = DOING_ANIM;
	}
}

void TreeAttackManager::attackSpecial()
{
	state = SPECIAL_ATTACK;

	rootAutoAim_->attack(false);
	lanternCols_->setDamaged(false);//raices especial si hacen da�o

	SoundEffect* s = &sdlutils().soundEffects().at("sfx_arbol_background");
	s->play();
	s->setChannelVolume(musicVolume_);

	treeCol_->setActive(false);
}

void TreeAttackManager::prepareToSpecial()
{
	if (!attacking_) {

		attacking_ = true;

		//pausa el timer del otro ataque y crea la lampara en el medio suspendida
		timerWave_->pause();

		lanternMov_->setActive(false);

		lanternTr_->getPos().set(Vector2D(sdlutils().width() / 2 - lanternTr_->getWidth() / 2, sdlutils().height() / 8));

		ParticleSystem* particlesys = new ParticleSystem(&sdlutils().images().at("luz_amarilla"), mngr_);
		particlesys->createParticlesLanternMove(20, lanternTr_->getPos().getX() + (lanternTr_->getWidth() / 2), sdlutils().height() - 50);

		ParticleSystem* particlesys2 = new ParticleSystem(&sdlutils().images().at("particula_simbolo2"), mngr_);
		particlesys2->createParticlesLanternMove(20, lanternTr_->getPos().getX() + (lanternTr_->getWidth() / 2), sdlutils().height() - 50);


		state = MOVING_TO_CENTER;

		movement_->moveToCenter();
	}
}