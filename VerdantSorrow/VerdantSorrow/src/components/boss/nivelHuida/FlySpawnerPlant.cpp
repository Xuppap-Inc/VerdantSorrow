#include "FlySpawnerPlant.h"
#include "../../../ecs/Entity.h"
#include "../../../ecs/manager.h"
#include "../../Transform.h"
#include "../wave/WaveMovement.h"
#include "../frog_boss/TongueAttack.h"
#include "../finalBoss/ClapAttack.h"
#include "../../player/PlayerAttributes.h"
#include "../../FramedImage.h"
#include "../../tutorial/TutorialFly.h"
#include "../frog_boss/FlyMovement.h"
#include "../frog_boss/FlyHp.h"


FlySpawnerPlant::FlySpawnerPlant(CollisionManager* colManager, bool lookingUp, float timer) : tr_(nullptr), colMan_(colManager), lookingUp_(lookingUp), cooldownSpawner_(timer)
{
}

FlySpawnerPlant::~FlySpawnerPlant()
{
}

void FlySpawnerPlant::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	vt = mngr_->addTimer();
	assert(tr_ != nullptr);
}

void FlySpawnerPlant::update()
{
	if (vt->currTime() > cooldownSpawner_) {
		vt->reset();
		auto fly = mngr_->addEntity();
		auto fTr = fly->addComponent<Transform>();
		float flyY;
		if (lookingUp_) flyY = tr_->getPos().getY() - 20;
		else flyY = tr_->getPos().getY() + tr_->getHeight() + 20;
		auto flyX = tr_->getPos().getX();

		fTr->init(Vector2D(flyX, flyY), Vector2D(), 60, 50, 0.0f);
		auto col = fly->addComponent<RectangleCollider>(fTr->getWidth(), fTr->getHeight());
		col->setIsTrigger(true);
		colMan_->addCollider(col);
		fly->addComponent<FramedImage>(&sdlutils().images().at("mosca"), 6, 6, (1000 / 30) * 31, 31, "mosca");
		fly->addComponent<FlyHp>();
		fly->addComponent<FlyMovement>();
		fly->addToGroup(ecs::_BOSSELEMENTS_GRP);
	}
}


