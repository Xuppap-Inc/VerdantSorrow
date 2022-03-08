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
									attackTimer_(0)
{
}

TreeAttackManager::~TreeAttackManager()
{
}

TreeAttackManager::TreeAttackManager(CollisionManager* collManager) : player_(), tr_(), collManager_(collManager), anim_(), attr_(), 
																	rootWidth_(0), rootAutoAim_(), rootWave_(), meleeAttack_(), attackTimer_(0)
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

	bool correct = tr_ != nullptr && player_ != nullptr && attr_ != nullptr && rootWave_ != nullptr && rootAutoAim_ != nullptr && meleeAttack_ != nullptr;
	assert(correct);

	phase = PHASE1;
	state = MOVING;
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

		//si se encuentra a distancia de ataque a melee, ataca
		if (absDistance < MELEE_ATTACK_DISTANCE) {

			meleeAttack_->attack(dir);
		}

		attackTimer_ += sdlutils().currRealTime();

		if (attackTimer_ >= TIME_BETWEEN_WAVES) state = WAVE;
	}

	else if (state == WAVE) {
	
		rootWave_->attack(dir);
	}
}
