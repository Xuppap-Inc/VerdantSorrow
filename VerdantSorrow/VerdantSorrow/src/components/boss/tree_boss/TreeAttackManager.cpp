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

TreeAttackManager::TreeAttackManager() : player_(), tr_(), collManager_(), anim_(), attr_(), rootWidth_(0), rootAutoAim_(), rootWave_(), meleeAttack_(),
									lastTimeWave_(0), attacking_(false)
{
}

TreeAttackManager::~TreeAttackManager()
{
}

TreeAttackManager::TreeAttackManager(CollisionManager* collManager) : player_(), tr_(), collManager_(collManager), anim_(), attr_(), 
																	rootWidth_(0), rootAutoAim_(), rootWave_(), meleeAttack_(), lastTimeWave_(0), 
																	attacking_(false)
{
}

void TreeAttackManager::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	player_ = mngr_->getHandler(ecs::_PLAYER)->getComponent<Transform>();
	attr_ = ent_->getComponent<BossAtributos>();

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

	lastTimeWave_ = sdlutils().currRealTime();
}

void TreeAttackManager::update()
{
	Vector2D playerPos = player_->getPos();
	Vector2D treePos = tr_->getPos();

	float distance = playerPos.getX() - treePos.getX();

	float absDistance = abs(distance);

	//direccion para los ataques
	int dir;

	if (distance > 0) dir = 1;
	else dir = -1;

	if (state == MOVING) {

		if (meleeAttack_->hasFinished()) attacking_ = false;

		//si se encuentra a distancia de ataque a melee, ataca
		if (absDistance < MELEE_ATTACK_DISTANCE) {

			SoundEffect* s = &sdlutils().soundEffects().at("sfx_arbol_attack");
			s->play();
			meleeAttack_->attack(dir);
			attacking_ = true;
		}

		int time = sdlutils().currRealTime();

		if (time - lastTimeWave_ >= TIME_BETWEEN_WAVES && !attacking_) {
			
			state = WAVE;
			rootWave_->attack(dir);
		}
	}

	else if (state == WAVE) {

		if (rootWave_->hasFinished()) {
			
			state = MOVING;

			lastTimeWave_ = sdlutils().currRealTime();
		}
	}
}
