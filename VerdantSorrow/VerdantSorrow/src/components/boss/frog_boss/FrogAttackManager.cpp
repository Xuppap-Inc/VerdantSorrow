#include "FrogAttackManager.h"
#include "../../../ecs/Entity.h"
#include "../../../ecs/Manager.h"
#include "FrogBigJump.h"
#include "FrogJump.h"
#include "TongueAttack.h"
#include "../../Transform.h"
#include "wave/WaveMovement.h"
#include "../../RectangleCollider.h"
#include "../../RectangleRenderer.h"
#include "../../../game/CollisionManager.h"
#include "../BossAtributos.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../FramedImage.h"

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
	tongueAttack_ = ent_->addComponent<TongueAttack>();
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
		if (jumpDirection_ == 1 && attr_->isOnRightBorder()) {
			anim_->flipX(false);
			jumpDirection_ = -1;
		}
		else if (jumpDirection_ == -1 && attr_->isOnLeftBorder()) {
			jumpDirection_ = 1;
			anim_->flipX(true);
		}
		
		//Si está saltando y tocando el suelo a la vez, para de saltar
		if (jumping_ && attr_->isOnGround()) {
			jumping_ = false;
			--jumpsUntilNextTongue_;
			//Start Idle animation

			//Si no quedan saltos hasta la proxima lengua, se cambia de estado a ataque con lengua.
			if (jumpsUntilNextTongue_ == 0) {
				//TODO spawn fly, tongue attack
				auto lengua_ = mngr_->addEntity();
				auto attr = lengua_->addComponent<Transform>();
				auto lado_ = 0;
				if (jumpDirection_ == -1)lado_ = tr_->getWidth();
			
				attr->init(Vector2D(tr_->getPos().getX()+lado_, tr_->getPos().getY()+ tr_->getHeight()/2), Vector2D(), 400, 200, 0.0f);
				lengua_->addComponent<FramedImage>(&sdlutils().images().at("lengua"), 1, 4, 2000, 4, "lengua");
				auto coll = lengua_->addComponent<RectangleCollider>(tr_->getWidth(), tr_->getHeight());
				coll->setIsTrigger(true);
				collManager_->addCollider(coll);

				std::cout << "Lenguetazo" << std::endl;
				jumpsUntilNextTongue_ = rand.nextInt(3, 5);
			}
		}
		
		if (!jumping_ && !jumpingBig_) {
			frogJump_->attack(jumpDirection_);
			//Start Jump animation
			anim_->changeanim(&sdlutils().images().at("ranajump"), 6, 6, 600, 32, "ranajump");
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
			anim_->changeanim(&sdlutils().images().at("ranaidle"), 4, 6, 1000,24, "ranaidle");
		}
		if (frogState_ != FLY_DIED) return;
		if (!jumping_ && !jumpingBig_) {
			bigJump_->attack(0); 
			jumpingBig_ = true;
			anim_->changeanim(&sdlutils().images().at("ranajump"), 6, 6, 700, 32, "ranajump");
			//Lanzar animación de salto grande
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
	fly_->addComponent<FramedImage>(&sdlutils().images().at("mosca"), 6, 6, 2000, 31, "mosca");
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

void FrogAttackManager::createWaves()
{
	createWave(1);
	createWave(-1);
}

void FrogAttackManager::onFlyDied() {
	frogState_ = FLY_DIED;
}
