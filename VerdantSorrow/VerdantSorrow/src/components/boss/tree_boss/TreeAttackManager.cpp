#include "TreeAttackManager.h"

#include "../../../ecs/Entity.h"
#include "../../../ecs/Manager.h"
#include "../../Transform.h"
#include "../../RectangleCollider.h"
#include "../../RectangleRenderer.h"
#include "../../../game/CollisionManager.h"
#include "../BossAtributos.h"
#include "RootWave.h"
#include "RootAutoAim.h"
#include "MeleeAttack.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../FramedImage.h"
#include "../../Image.h"
#include "TreeMovement.h"
#include "LanternSpawner.h"

TreeAttackManager::TreeAttackManager() : player_(), tr_(), collManager_(), anim_(), attr_(), rootWidth_(0), rootAutoAim_(), rootWave_(), meleeAttack_(),
timerWave_(), attacking_(false), timerSpecial_(), img_(), treeCol_(), waiting_(false), lanternSpawner_()
{
}

TreeAttackManager::~TreeAttackManager()
{
}

TreeAttackManager::TreeAttackManager(CollisionManager* collManager) : player_(), tr_(), collManager_(collManager), anim_(), attr_(), 
																	rootWidth_(0), rootAutoAim_(), rootWave_(), meleeAttack_(), timerWave_(), 
																	attacking_(false), timerSpecial_(), img_(), treeCol_(), waiting_(false),lanternSpawner_()
{
}

void TreeAttackManager::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	player_ = mngr_->getHandler(ecs::_PLAYER)->getComponent<Transform>();
	attr_ = ent_->getComponent<BossAtributos>();
	img_ = ent_->getComponent<Image>();
	treeCol_ = ent_->getComponent<RectangleCollider>();
	movement_ = ent_->getComponent<TreeMovement>();
	lanternSpawner_ = ent_->getComponent<LanternSpawner>();

	rootWave_ = ent_->getComponent<RootWave>();
	rootAutoAim_ = ent_->getComponent<RootAutoAim>();
	meleeAttack_ = ent_->getComponent<MeleeAttack>();

	musicaFase2_ = &sdlutils().musics().at("musica_linterna_fase2");
	musicaFase2_->play();
	musicaFase2_->setMusicVolume(0);

	musicaFase1_ = &sdlutils().soundEffects().at("musica_linterna_fase1");
	musicaFase1_->play(10, 0);
	musicaFase1_->setChannelVolume(80, 0);

	bool correct = tr_ != nullptr && player_ != nullptr && attr_ != nullptr && rootWave_ != nullptr && rootAutoAim_ != nullptr && meleeAttack_ != nullptr;
	assert(correct);

	phase = PHASE1;
	state = MOVING;

	timerWave_.reset();
	timerSpecial_.reset();
}

void TreeAttackManager::update()
{
	Vector2D playerPos = player_->getPos();
	Vector2D treePos = tr_->getPos();

	float distance = playerPos.getX() - treePos.getX();

	float absDistance = abs(distance);

	//direccion para los ataques

	if (!attacking_) {
		if (distance > 0) dir_ = 1;
		else dir_ = -1;
	}

	if (state == MOVING) {

		if (meleeAttack_->hasFinished()) attacking_ = false, newAtack_ = true;

		//si se encuentra a distancia de ataque a melee, ataca
		if (absDistance < MELEE_ATTACK_DISTANCE && newAtack_) {

			SoundEffect* s = &sdlutils().soundEffects().at("sfx_arbol_attack");
			s->play();
			meleeAttack_->attack(dir_);
			attacking_ = true;
			newAtack_ = false;
		}

		if (timerWave_.currTime() > TIME_BETWEEN_WAVES) attackWave(dir_);

		if (timerSpecial_.currTime() > TIME_FOR_SPECIAL) prepareToSpecial();

		
	}

	else if (state == WAVE) {

		if (rootWave_->hasFinished()) {
			
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
			img_->setVisible(true);
			treeCol_->setActive(true);

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

			waitTimer_.reset();
			waitTimer_.pause();
		}
	}

	else if (state == MOVING_TO_CENTER) {
	
		if (movement_->hasFinishedMovingToCenter()) {
		
			attackSpecial();
			
		}
	}
}

void TreeAttackManager::returnToIni()
{
	auto treeX = sdlutils().width() / 2 - 80;
	auto treeY = sdlutils().height() - 320;
	tr_->getPos().set(Vector2D(treeX, treeY));
}

void TreeAttackManager::attackWave(int dir)
{
	if (!attacking_) {

		//pausa el timer del otro ataque
		timerSpecial_.pause();

		state = WAVE;
		rootWave_->attack(dir);
	}
}

void TreeAttackManager::attackSpecial()
{
	state = SPECIAL_ATTACK;

	rootAutoAim_->attack();

	img_->setVisible(false);
	treeCol_->setActive(false);
}

void TreeAttackManager::prepareToSpecial()
{
	if (!attacking_) {

		attacking_ = true;

		//pausa el timer del otro ataque y crea la lampara en el medio suspendida
		timerWave_.pause();
		lanternSpawner_->createLantern(sdlutils().width()-100, sdlutils().height()-50,true);
		lanternSpawner_->createLantern(sdlutils().width() / 2, sdlutils().height() / 3,true);
		lanternSpawner_->createLantern(20, sdlutils().height()-50,true);

		state = MOVING_TO_CENTER;

		movement_->moveToCenter();
	}
}

