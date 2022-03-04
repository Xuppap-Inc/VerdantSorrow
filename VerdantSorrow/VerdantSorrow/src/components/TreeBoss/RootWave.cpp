#include "RootWave.h"

#include "../../sdlutils/SDLUtils.h"
#include "../../ecs/Entity.h"
#include "../../ecs/Manager.h"
#include "../Transform.h"
#include "../RectangleCollider.h"
#include "../RectangleRenderer.h"
#include "../../game/CollisionManager.h"
#include "../FrogBoss/BossAtributos.h"

RootWave::RootWave() : tr_(), lastTime_(0), rootSpawner_(), attacking_(false), dir_(1), rootPos_(-1), rootW_(0), timeBetweenRoots_(500)
{
}

RootWave::~RootWave()
{
}

void RootWave::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	rootSpawner_ = ent_->getComponent<RootSpawner>();

	bool comps = tr_ != nullptr && rootSpawner_ != nullptr;
	assert(comps);
}

void RootWave::update()
{
	if (attacking_ && sdlutils().currRealTime() - lastTime_ > timeBetweenRoots_) {
	
		rootSpawner_->createRoot(rootPos_);
		rootPos_ += (rootW_ + 10) * dir_;

		lastTime_ = sdlutils().currRealTime();

		if (rootPos_ < 0 || rootPos_ > sdlutils().width()) attacking_ = false;
	}
}

void RootWave::attack(int dir)
{
	//inicializa variables
	rootPos_ = tr_->getPos().getX();
	dir_ = dir;
	attacking_ = true;
	rootW_ = rootSpawner_->getRootWidth();

	//crea la primera raíz
	rootSpawner_->createRoot(rootPos_);
	rootPos_ += (rootW_ + 5) * dir;
	lastTime_ = sdlutils().currRealTime();
}
