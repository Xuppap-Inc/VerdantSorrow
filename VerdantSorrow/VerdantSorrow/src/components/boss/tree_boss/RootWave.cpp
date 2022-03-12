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


RootWave::RootWave() : tr_(), lastTime_(0), rootSpawner_(), attacking_(false), 
dir_(1), rootPos_(-1), rootW_(0), treeMovement_(),lanternSpawner_(),lanternPos_(-1)
{
}

RootWave::~RootWave()
{
}

void RootWave::initComponent()
{
	//nuevo feat
	lanternSpawner_ = ent_->getComponent<LanternSpawner>();
	tr_ = ent_->getComponent<Transform>();
	rootSpawner_ = ent_->getComponent<RootSpawner>();
	treeMovement_ = ent_->getComponent<TreeMovement>();

	

	bool comps = tr_ != nullptr && rootSpawner_ != nullptr && treeMovement_ != nullptr
		 && lanternSpawner_ != nullptr  ;
	assert(comps);
}

void RootWave::update()
{
	//si ha pasado el tiempo entre raices
	if (attacking_ && sdlutils().currRealTime() - lastTime_ > TIME_BETWEEN_ROOTS) {
	
		//crea la raiz y suma la posicion de la siguiente
		rootSpawner_->createRoot(rootPos_);
		rootPos_ += (rootW_ + SPACE_BETWEEN_ROOTS) * dir_;

		lastTime_ = sdlutils().currRealTime();
		
		//si llega al borde de la pantalla acaba el ataque y activa el movimiento
		if (rootPos_ < 0 || rootPos_ > sdlutils().width()) {
			
			attacking_ = false;
			
		}
	}
}

void RootWave::attack(int dir)
{
	//inicializa variables
	rootPos_ = tr_->getPos().getX();
	dir_ = dir;
	attacking_ = true;
	rootW_ = rootSpawner_->getRootWidth();

	//para el movimiento del arbol
	treeMovement_->setMoveActive(false);

	//crea la primera raíz
	rootSpawner_->createRoot(rootPos_);
	rootPos_ += (rootW_ + SPACE_BETWEEN_ROOTS) * dir;
	lastTime_ = sdlutils().currRealTime();

	
}
