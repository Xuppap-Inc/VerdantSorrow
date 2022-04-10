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

TreeAttackManager::TreeAttackManager() : player_(), tr_(), collManager_(), anim_(), rootWidth_(0), rootAutoAim_(), rootWave_(), meleeAttack_(),
timerWave_(), attacking_(false), timerSpecial_(), treeCol_(), waiting_(false), lantern_(), lanternTr_(), lanternMov_(), lanternCols_(), attribs_(), dir_(0), movement_(),timerCd_()
{
}

TreeAttackManager::~TreeAttackManager()
{
}

TreeAttackManager::TreeAttackManager(CollisionManager* collManager) : player_(), tr_(), collManager_(collManager), anim_(), 
																	rootWidth_(0), rootAutoAim_(), rootWave_(), meleeAttack_(), timerWave_(), 
																	attacking_(false), timerSpecial_(), treeCol_(), waiting_(false), 
																	lantern_(), lanternTr_(), lanternMov_(), lanternCols_(), attribs_(), dir_(0), movement_(),timerCd_()
{
}

void TreeAttackManager::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	player_ = mngr_->getHandler(ecs::_PLAYER)->getComponent<Transform>();
	treeCol_ = ent_->getComponent<RectangleCollider>();
	movement_ = ent_->getComponent<TreeMovement>();
	anim_ = ent_->getComponent<FramedImage>();

	lantern_ = mngr_->getHandler(ecs::_LANTERN);
	lanternTr_ = lantern_->getComponent<Transform>();
	lanternMov_ = lantern_->getComponent<LanternMovement>();
	lanternCols_ = lantern_->getComponent<LanternCollisions>();
	attribs_ = lantern_->getComponent<BossAtributos>();

	rootWave_ = ent_->getComponent<RootWave>();
	rootAutoAim_ = ent_->getComponent<RootAutoAim>();
	meleeAttack_ = ent_->getComponent<MeleeAttack>();

	musicaFase2_ = &sdlutils().musics().at("musica_linterna_fase2");
	musicaFase2_->play();
	musicaFase2_->setMusicVolume(0);

	musicaFase1_ = &sdlutils().soundEffects().at("musica_linterna_fase1");
	musicaFase1_->play(10, 0);
	musicaFase1_->setChannelVolume(80, 0);

	bool correct = tr_ != nullptr && player_ != nullptr && rootWave_ != nullptr && rootAutoAim_ != nullptr && meleeAttack_ != nullptr && lanternTr_ != nullptr;
	assert(correct);

	phase = PHASE1;
	state = MOVING;

	timerWave_.reset();
	timerSpecial_.reset();
	timerCd_.reset();
	
}

void TreeAttackManager::update()
{
	Vector2D playerPos = player_->getPos();
	Vector2D treePos = tr_->getPos();

	//el arbol es más ancho que su collider, se ajusta jugando con su ancho
	float distance = playerPos.getX() - treePos.getX()-tr_->getWidth()/7;

	float absDistance = abs(distance);

	if (!attacking_) {
		if (distance > 0) dir_ = 1;
		else dir_ = -1;
	}

	if (state == MOVING) {

		if (meleeAttack_->hasFinished()) attacking_ = false, newAtack_ = true;

		if (!attacking_) animNewState_ = ANIM_WALK;
		anim_->repeat(true);

		
		
		//si se encuentra a distancia de ataque a melee, ataca
		if (((dir_<0 &&absDistance < MELEE_ATTACK_DISTANCE) || (dir_ > 0 && absDistance<tr_->getWidth() + MELEE_ATTACK_DISTANCE))  && newAtack_) {
			std::cout << timerCd_.currTime() << std::endl;
			std::cout << distance << std::endl;
			std::cout << tr_->getWidth() << std::endl;
			
			if(timerCd_.currTime() >= ATTACK_CD) {
				std::cout << dir_ << std::endl;
				animNewState_ = ANIM_ATTACK;
				anim_->repeat(false);

				SoundEffect* s = &sdlutils().soundEffects().at("sfx_arbol_attack");
				s->play();
				meleeAttack_->attack(dir_);
				attacking_ = true;
				newAtack_ = false;
				timerCd_.reset();
			}
			
		}

		if (phase == PHASE1) {

			if (attribs_->getLife() <= attribs_->getMaxHp() / 2) {

				animNewState_ = ANIM_CHANGE_PHASE;
				anim_->repeat(false);

				//callback que llama al cambio de estado al acabar la animacion de cambio de fase
				std::function<void()> changePhaseCallback = [this]() { state = MOVING; };
				anim_->registerEvent(std::pair<int, std::string>(13, "arbol_capa_cambio_fase"), changePhaseCallback);

				SoundEffect* s = &sdlutils().soundEffects().at("sfx_cambio_fase");
				s->play();
				musicaFase2_->setMusicVolume(100);
				musicaFase1_->pauseChannel(0);
				phase = PHASE2;

				rootAutoAim_->attack(true);

				lanternMov_->setActive(false);
				lanternMov_->moveToSide();

				lanternCols_->changeToSecondPhase();
				rootAutoAim_->changeToSecondPhase();

				state = CHANGING_PHASE;
			}

			else {

				if (timerWave_.currTime() > TIME_BETWEEN_WAVES) attackWave();

				if (timerSpecial_.currTime() > TIME_FOR_SPECIAL) prepareToSpecial();
			}
		}
	}

	else if (state == WAVE) {
		lanternCols_->setDamaged(true); //waves no hacen da�o
		if (rootWave_->getMove()) {
			
			state = MOVING;

			movement_->setMoveActive(true);

			//timers
			timerWave_.reset();
			timerSpecial_.resume();

			attacking_ = false;
		}
	}

	else if (state == SPECIAL_ATTACK) {
		
		if (!waiting_ && rootAutoAim_->hasFinished()) {

			//reactiva al arbol
			lanternMov_->setActive(true);
			anim_->setVisible(true);

			lanternCols_->setDamaged(true);

			//lo devuelve al centro
			returnToIni();

			waitTimer_.reset();

			waiting_ = true;
		}

		else if (waiting_ && waitTimer_.currTime() > WAIT_AFTER_SPECIAL) {

			state = MOVING;

			//timers
			timerSpecial_.reset();
			timerWave_.resume();

			attacking_ = false;
			movement_->setMoveActive(true);
			lanternMov_->setActive(true);
			treeCol_->setActive(true);

			waitTimer_.reset();
			waitTimer_.pause();
		}
	}

	else if (state == MOVING_TO_CENTER) {
	
		if (movement_->hasFinishedMovingToCenter()) {
		
			animNewState_ = ANIM_BACKGROUND;
			anim_->repeat(false);

			//callback que llama al ataque especial al acabar la animacion
			std::function<void()> attackCallback = [this]() { attackSpecial(); };

			anim_->registerEvent(std::pair<int, std::string>(8, "arbol_capa_background"), attackCallback);
		}
	}

	if (animState_ != animNewState_) {
		animState_ = animNewState_;
		switch (animState_)
		{
		case TreeAttackManager::ANIM_IDLE:
			if (phase==PHASE2)anim_->changeanim(&sdlutils().images().at("arbol_idle"), 5, 6, (1000 / 30) * 27, 27, "arbol_idle");
			else anim_->changeanim(&sdlutils().images().at("arbol_capa_idle"), 5, 6, (1000 / 30) * 25, 25, "arbol_capa_idle");
			break;
		case TreeAttackManager::ANIM_WALK:
			if (phase==PHASE2)anim_->changeanim(&sdlutils().images().at("arbol_walk"), 5, 6, (1000 / 30) * 28, 28, "arbol_walk");
			else anim_->changeanim(&sdlutils().images().at("arbol_capa_walk"), 2, 6, (1000 / 30) * 12, 12, "arbol_capa_walk");
			break;
		case TreeAttackManager::ANIM_ATTACK:
			anim_->setColor(255, 200, 20, 200);
			if (phase==PHASE2)anim_->changeanim(&sdlutils().images().at("arbol_attack"), 4, 6, (1000 / 30) * 24, 24, "arbol_attack");
			else anim_->changeanim(&sdlutils().images().at("arbol_capa_attack"), 4, 6, (1000 / 30) * 24, 24, "arbol_capa_attack");
			break;
		case TreeAttackManager::ANIM_ATTACK_COMBO:
			anim_->setColor(255, 200, 20, 200);
			if (phase==PHASE2)anim_->changeanim(&sdlutils().images().at("arbol_attack"), 4, 6, (1000 / 30) * 24, 24, "arbol_attack");
			else anim_->changeanim(&sdlutils().images().at("arbol_capa_attack_combo"), 3, 6, (1000 / 30) * 13, 13, "arbol_capa_attack_combo");
			break;
		case TreeAttackManager::ANIM_ROOTS:
			anim_->setColor(255, 200, 20, 500);
			anim_->changeanim(&sdlutils().images().at("arbol_capa_roots"), 3, 6, (1000 / 30) * 16, 16, "arbol_capa_roots");
			break;
		case TreeAttackManager::ANIM_BACKGROUND:
			anim_->changeanim(&sdlutils().images().at("arbol_capa_background"), 2, 6, (1000 / 30) * 9, 9, "arbol_capa_background");
			break;
		case TreeAttackManager::ANIM_CHANGE_PHASE:
			anim_->changeanim(&sdlutils().images().at("arbol_capa_cambio_fase"), 3, 6, (1000 / 30) * 14, 14, "arbol_capa_cambio_fase");
			break;
		case TreeAttackManager::ANIM_DEATH:
			anim_->changeanim(&sdlutils().images().at("arbol_muerte"), 3, 6, (1000 / 30) * 16, 16, "arbol_muerte");
			break;
		default:
			break;
		}
	}
}

void TreeAttackManager::returnToIni()
{
	auto treeX = sdlutils().width() / 2 - 80;
	auto treeY = sdlutils().height() - 360;
	tr_->getPos().set(Vector2D(treeX, treeY));
}

void TreeAttackManager::attackWave()
{
	if (!attacking_) {

		animNewState_ = ANIM_ROOTS;
		anim_->repeat(false);

		//pausa el timer del otro ataque
		timerSpecial_.pause();
		
		//callback que llama al ataque de raices al acabar la animacion
		std::function<void()> attackCallback = [this]() { rootWave_->attack(dir_); };

		anim_->registerEvent(std::pair<int, std::string>(15, "arbol_capa_roots"), attackCallback);

		state = WAVE;
	}
}

void TreeAttackManager::attackSpecial()
{
	state = SPECIAL_ATTACK;

	rootAutoAim_->attack(false);
	lanternCols_->setDamaged(false);//raices especial si hacen da�o

	anim_->setVisible(false);
	treeCol_->setActive(false);
}

void TreeAttackManager::prepareToSpecial()
{
	if (!attacking_) {

		attacking_ = true;

		//pausa el timer del otro ataque y crea la lampara en el medio suspendida
		timerWave_.pause();

		lanternMov_->setActive(false);

		lanternTr_->getPos().set(Vector2D(sdlutils().width() / 2 - lanternTr_->getWidth() / 2, sdlutils().height() / 8));

		state = MOVING_TO_CENTER;

		movement_->moveToCenter();
	}
}

