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
#include "../../../sdlutils/SDLUtils.h"
#include "../../FramedImage.h"

TreeAttackManager::TreeAttackManager() : player_(), tr_(), collManager_(), anim_(), attr_(), rootWidth_(0), rootAutoAim_(), rootWave_()
{
}

TreeAttackManager::~TreeAttackManager()
{
}

TreeAttackManager::TreeAttackManager(CollisionManager* collManager) : player_(), tr_(), collManager_(collManager), anim_(), attr_(), rootWidth_(0), rootAutoAim_(), rootWave_()
{
}

void TreeAttackManager::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	player_ = mngr_->getHandler(ecs::_PLAYER)->getComponent<Transform>();
	attr_ = ent_->getComponent<BossAtributos>();

	rootWave_ = ent_->getComponent<RootWave>();
	rootAutoAim_ = ent_->getComponent<RootAutoAim>();
	bool correct = tr_ != nullptr && player_ != nullptr && attr_ != nullptr && rootWave_ != nullptr && rootAutoAim_ != nullptr;
	assert(correct);

	//rootWave_->attack(1);
	rootAutoAim_->attack();
}

void TreeAttackManager::update()
{
}
