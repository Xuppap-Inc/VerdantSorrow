#include "RootWave.h"

#include "../../../sdlutils/SDLUtils.h"
#include "../../../ecs/Entity.h"
#include "../../../ecs/Manager.h"
#include "../../Transform.h"
#include "../../RectangleCollider.h"
#include "../../RectangleRenderer.h"
#include "../../../game/CollisionManager.h"
#include "../BossAtributos.h"
#include "TreeMovement.h"


RootWave::RootWave() : tr_(), rootSpawnTimer_(), rootSpawner_(), attacking_(false), 
dir_(1), rootPos_(-1), rootW_(0), treeMovement_(), ableToMove_(false), movingTime_(10000), waitTimer_()
{
}

RootWave::~RootWave()
{
}

void RootWave::initComponent()
{
	
	tr_ = ent_->getComponent<Transform>();
	rootSpawner_ = ent_->getComponent<RootSpawner>();
	treeMovement_ = ent_->getComponent<TreeMovement>();
	rootSpawnTimer_ = mngr_->addTimer();
	waitTimer_ = mngr_->addTimer();

	bool comps = tr_ != nullptr && rootSpawner_ != nullptr && treeMovement_ != nullptr;
	assert(comps);
}

void RootWave::update()
{
	//si ha pasado el tiempo entre raices
	if (attacking_ && rootSpawnTimer_->currTime() > TIME_BETWEEN_ROOTS) {
	
		//crea la raiz y suma la posicion de la siguiente
		rootSpawner_->createRoot(rootPos_);
		nRoots_++;
		rootPos_ += (rootW_ + SPACE_BETWEEN_ROOTS) * dir_;

		rootSpawnTimer_->reset();
		//si llega al borde de la pantalla acaba el ataque y activa el movimiento
		if (rootPos_ < 0 || rootPos_ > sdlutils().width()) {
			
			waitTimer_->reset();			
			attacking_ = false;
		}		
	}
	//si ha pasado el doble de tiempo de lo que dura la oleada
	//(que es lo que tarda en llegar la última al limite)
	else if (!attacking_ && waitTimer_->currTime() >= TIME_UNTIL_MOVING) {
		ableToMove_ = true;
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
	rootPos_ += (rootW_ + SPACE_BETWEEN_ROOTS) * dir;
	rootSpawnTimer_->reset();
	ableToMove_ = false;	
	waitTimer_->reset();
	waitTimer_->pause();

	nRoots_ = 0;
}


