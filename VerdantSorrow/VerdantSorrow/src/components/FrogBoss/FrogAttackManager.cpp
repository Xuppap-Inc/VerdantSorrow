#include "FrogAttackManager.h"
#include "../../ecs/Entity.h"
#include "../../ecs/Manager.h"
#include "FrogBigJump.h"
#include "FrogJump.h"
#include "TongueAttack.h"
#include "../Transform.h"
#include "../wave/WaveMovement.h"
#include "../RectangleCollider.h"
#include "../RectangleRenderer.h"
#include "../../game/CollisionManager.h"

FrogAttackManager::FrogAttackManager() : frogJump_(), bigJump_(), fly_(), player_(), tr_(),
				frogState_(FIRST_PHASE), jumping_(), jumpDirection_(1), jumpsUntilNextTongue_(0), flySpacing_(0)
{
}

FrogAttackManager::FrogAttackManager(CollisionManager* collManager) : frogJump_(), bigJump_(), fly_(), player_(), tr_(), collManager_(collManager),
					frogState_(FIRST_PHASE), jumping_(), jumpDirection_(1), jumpsUntilNextTongue_(0), flySpacing_(0)
{
}

FrogAttackManager::~FrogAttackManager()
{
}

void FrogAttackManager::initComponent()
{
	frogJump_ = ent_->addComponent<FrogJump>(30);
	bigJump_ = ent_->addComponent<FrogBigJump>(40);
	tongueAttack_ = ent_->addComponent<TongueAttack>();
	tr_ = ent_->getComponent<Transform>();
	player_ = mngr_->getHandler(ecs::_PLAYER);
	bool correct = tr_ != nullptr && frogJump_ != nullptr && tongueAttack_ != nullptr && bigJump_ != nullptr && player_ != nullptr;
	assert(correct);
}

void FrogAttackManager::update()
{
}

ecs::Entity* FrogAttackManager::createFly()
{
	fly_ = mngr_->addEntity();
	fly_->addComponent<Transform>();
	return fly_;;
}

void  FrogAttackManager::createWave(int dir)
{
	//Se crea la onda expansiva
	auto Wave = mngr_->addEntity();
	//Se añade el transform
	auto WaveTr = Wave->addComponent<Transform>();
	auto WaveX = tr_->getPos().getX();
	auto WaveY = sdlutils().height() - 50;
	//dir = {-1, 1}
	auto WaveDir = dir;
	auto WaveSpeed = 5;
	//Se le dan las posiciones iniciales, velocidad, ancho y alto a la onda
	WaveTr->init(Vector2D(WaveX, WaveY), Vector2D(), 150, 50, 0.0f);
	//Se le añade un color inicial a la onda
	Wave->addComponent<RectangleRenderer>(SDL_Color());

	//Se añade un collider a la onda
	auto waveCollider = Wave->addComponent<RectangleCollider>(WaveTr->getWidth(), WaveTr->getHeight());
	waveCollider->setIsTrigger(true);
	//Se añade el collider al colliderGameManager
	collManager_->addCollider(waveCollider);
	//Se añade el movimiento horizontal
	Wave->addComponent<WaveMovement>(WaveDir, WaveSpeed);

}
