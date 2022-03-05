#include "../../ecs/Entity.h"
#include "../../ecs/Manager.h"
#include "../Transform.h"
#include "../RectangleCollider.h"
#include "../RectangleRenderer.h"
#include "../../game/CollisionManager.h"
#include "../../sdlutils/SDLUtils.h"
#include "../FramedImage.h"
#include "../BossComponents.h"

FrogAttackManager::FrogAttackManager() : frogJump_(), bigJump_(), fly_(), player_(), tr_(),
				frogState_(FLY_DIED), jumping_(false), jumpingBig_(false), jumpDirection_(-1), jumpsUntilNextTongue_(0), flySpacing_(0), collManager_(), tongueAttack_()
{
}

FrogAttackManager::FrogAttackManager(CollisionManager* collManager) : frogJump_(), bigJump_(), fly_(), player_(), tr_(), collManager_(collManager),
					frogState_(FLY_DIED), jumping_(false), jumpingBig_(false), jumpDirection_(1), jumpsUntilNextTongue_(0), flySpacing_(0), tongueAttack_()
{
}

FrogAttackManager::~FrogAttackManager()
{
}

void FrogAttackManager::initComponent()
{
	frogJump_ = ent_->addComponent<FrogJump>(30);
	bigJump_ = ent_->addComponent<FrogBigJump>(40);
	tongueAttack_ = ent_->addComponent<TongueAttack>(collManager_);
	tr_ = ent_->getComponent<Transform>();
	player_ = mngr_->getHandler(ecs::_PLAYER)->getComponent<Transform>();
	attr_ = ent_->getComponent<BossAtributos>();
	anim_ = ent_->getComponent<FramedImage>();
	
	bool correct = tr_ != nullptr && frogJump_ != nullptr && tongueAttack_ != nullptr && bigJump_ != nullptr && player_ != nullptr;	
	assert(correct);
}

//Patrones de ataque de la rana
void FrogAttackManager::update()
{
	auto& rand = sdlutils().rand();
	if (frogState_ == FIRST_PHASE) {
		//Cambio en la dirección del salto si choca con bordes
		
		
		//Si está saltando y tocando el suelo a la vez, para de saltar
		if (jumping_ && attr_->isOnGround()) {
			jumping_ = false;
			--jumpsUntilNextTongue_;
			//Start Idle animation

			//Si no quedan saltos hasta la proxima lengua, se cambia de estado a ataque con lengua.
			if (jumpsUntilNextTongue_ == 0) {
				//TODO spawn fly, tongue attack
				std::cout << "Lenguetazo" << std::endl;
				jumpsUntilNextTongue_ = rand.nextInt(3, 5);
			}
		}
		
		if (!jumping_ && !jumpingBig_) {
			frogJump_->attack(jumpDirection_);
			//Start Jump animation
			anim_->changeanim(&sdlutils().images().at("ranajump"), 6, 6, 500, 31);
			//Cambio de estado a saltando
			jumping_ = true;
		}
		
		

	}else if (FLY_DIED) {
		//Cambio a sprite enfadado
		if (jumpingBig_ && attr_->isOnGround()) {
			jumpingBig_ = false;
			createWaves();
			frogState_ = FIRST_PHASE;
			//Start Idle animation
		}
		if (frogState_ != FLY_DIED) return;
		if (!jumping_ && !jumpingBig_) {
			bigJump_->attack(0); 
			jumpingBig_ = true;
			//Lanzar animación de salto grande
		}
	}
	else if (SECOND_PHASE) {
		auto jump = rand.nextInt(0, 100);
	}

	if (jumpDirection_ == 1 && attr_->isOnRightBorder()) {
		anim_->flipX(false);
		jumpDirection_ = -1;
	}
	else if (jumpDirection_ == -1 && attr_->isOnLeftBorder()) {
		jumpDirection_ = 1;
		anim_->flipX(true);
	}

	switch (frogState_) {
		case JUMPING:
			if (attr_->isOnGround()) {
				frogState_ = WAITING;
				jumping_ = false;
				jumpsUntilNextTongue_--;
			}
			break;
		case JUMPING_BIG:
			if (attr_->isOnGround()) {
				frogState_ = WAITING;
				jumpingBig_ = false;
				if (secondPhase_) jumpsUntilNextTongue_--;
				createWaves();
			}
			break;
		case CALC_NEXT_ATTACK:
			if (jumpsUntilNextTongue_ == 0) {
				std::cout << "Lenguetazo" << std::endl;
				jumpsUntilNextTongue_ = rand.nextInt(3, 5);
			}
			break;
		case WAITING:
			if (jumpsDelay_ + lastUpdate_ < sdlutils().currRealTime()) {

			}
			break;
		case FLY_DIED:
			if (!jumping_ && !jumpingBig_) {
				bigJump_->attack(0);
				jumpingBig_ = true;
				//Lanzar animacion de salto largo
			}
			break;
		case SECOND_PHASE:
			break;
		default:
			break;
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
	mngr_->setHandler(ecs::_FLY, fly_);
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
	if (WaveDir == 1) {
		WaveX += tr_->getWidth();
	}
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

void FrogAttackManager::createWaves()
{
	createWave(1);
	createWave(-1);
}

void FrogAttackManager::onFlyDied() {
	frogState_ = FLY_DIED;
}

