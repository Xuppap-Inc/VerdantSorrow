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
#include "BossAtributos.h"
#include "../../sdlutils/SDLUtils.h"

FrogAttackManager::FrogAttackManager() : frogJump_(), bigJump_(), fly_(), player_(), tr_(),
				frogState_(FIRST_PHASE), jumping_(), jumpDirection_(1), jumpsUntilNextTongue_(0), flySpacing_(0), collManager_(), tongueAttack_()
{
}

FrogAttackManager::FrogAttackManager(CollisionManager* collManager) : frogJump_(), bigJump_(), fly_(), player_(), tr_(), collManager_(collManager),
					frogState_(FIRST_PHASE), jumping_(), jumpDirection_(1), jumpsUntilNextTongue_(0), flySpacing_(0), tongueAttack_()
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
	player_ = mngr_->getHandler(ecs::_PLAYER)->getComponent<Transform>();
	attr_ = ent_->getComponent<BossAtributos>();
	bool correct = tr_ != nullptr && frogJump_ != nullptr && tongueAttack_ != nullptr && bigJump_ != nullptr && player_ != nullptr;	
	assert(correct);
}

//Patrones de ataque de la rana
void FrogAttackManager::update()
{
	auto& rand = sdlutils().rand();
	if (frogState_ == FIRST_PHASE) {
		if (jumpDirection_ == 1 && attr_->isOnRightBorder()) {
			std::cout << "hacia la izq" << std::endl;
			jumpDirection_ == -1;
		}
		else if (jumpDirection_ == -1 && attr_->isOnLeftBorder()) {
			std::cout << "hacia la der" << std::endl;
			jumpDirection_ == 1;
		}
		
		
		if (jumping_ && attr_->isOnGround()) {
			std::cout << "parando" << std::endl;
			jumping_ = false;
			--jumpsUntilNextTongue_;
			if (jumpsUntilNextTongue_ == 0) {
				//TODO spawn fly, tongue attack
				std::cout << "Lenguetazo" << std::endl;
				jumpsUntilNextTongue_ = rand.nextInt(3, 5);
			}
		}
		if (!jumping_) {
			std::cout << "saltando" << std::endl;
			frogJump_->attack(jumpDirection_);
			jumping_ = true;
		}
		
		

	}else if (FLY_DIED) {
		//Cambio a sprite enfadado
		if (!jumping_ && !jumpingBig_) {
			bigJump_->attack(0); 			
		}
	}
	else if (SECOND_PHASE) {
		auto jump = rand.nextInt(0, 100);
	}

}

ecs::Entity* FrogAttackManager::createFly()
{
	fly_ = mngr_->addEntity();
	auto attr = fly_->addComponent<Transform>();
	attr->init(Vector2D(player_->getPos().getX(), player_->getPos().getY()), Vector2D(), 100, 100, 0.0f);
	auto coll = fly_->addComponent<RectangleCollider>(tr_->getWidth(), tr_->getHeight());
	coll->setIsTrigger(true);
	collManager_->addCollider(coll);
	fly_->addComponent<RectangleRenderer>();
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

void FrogAttackManager::onFlyDied() {
	frogState_ = FLY_DIED;
}
