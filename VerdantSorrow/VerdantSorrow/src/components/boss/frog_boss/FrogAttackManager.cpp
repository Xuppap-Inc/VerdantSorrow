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
#include "FlyHp.h"

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
	if (attr_->getLife() <= 0) {
		std::cout << "Muerte" << std::endl;
		ent_->setAlive(false);
		return;
	}
	flipOnBorders();
	switch (frogState_) {
		case JUMPING:
			if (attr_->isOnGround()) {
				onGrounded(jumping_, false);
			}
			break;
		case JUMPING_BIG:
			if (attr_->isOnGround()) {
				onGrounded(jumpingBig_, true);
			}
			break;
		case TONGUE:
			/*if (tongueAttack_->finished()) {
				
			}*/
			frogState_ = WAITING;
			delay_ = rand.nextInt(1000, 3000);
			lastUpdate_ = sdlutils().currRealTime();
			break;
		case CALC_NEXT_ATTACK:
			nextAttack();
			break;
		case WAITING:
			//std::cout << "esperando" << std::endl;
			if (delay_ + lastUpdate_ < sdlutils().currRealTime()) {
				frogState_ = CALC_NEXT_ATTACK;
			}
			break;
		case FLY_DIED:
			if (!jumping_ && !jumpingBig_) {
				bigJump_->attack(0);
				//tongueAttack_->cancel();
				jumpingBig_ = true;
				frogState_ = JUMPING_BIG;
				angry_ = true;
				//Lanzar animacion de salto largo
			}
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
	fly_->addComponent<FramedImage>(&sdlutils().images().at("mosca"), 6, 6, 2000, 31, "mosca");
	fly_->addComponent<FlyHp>(this);
	mngr_->setHandler(ecs::_FLY, fly_);
	return fly_;;
}

void  FrogAttackManager::createWave(int dir)
{
	//Se crea la onda expansiva
	auto Wave = mngr_->addEntity();
	//Se a�ade el transform
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
	//Se le a�ade un color inicial a la onda
	Wave->addComponent<RectangleRenderer>(SDL_Color());

	//Se a�ade un collider a la onda
	auto waveCollider = Wave->addComponent<RectangleCollider>(WaveTr->getWidth(), WaveTr->getHeight());
	waveCollider->setIsTrigger(true);
	//Se a�ade el collider al colliderGameManager
	collManager_->addCollider(waveCollider);
	//Se a�ade el movimiento horizontal
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

void FrogAttackManager::flipOnBorders()
{
	if (jumpDirection_ == 1 && attr_->isOnRightBorder()) {
		anim_->flipX(false);
		jumpDirection_ = -1;
	}
	else if (jumpDirection_ == -1 && attr_->isOnLeftBorder()) {
		jumpDirection_ = 1;
		anim_->flipX(true);
	}
}

void FrogAttackManager::onGrounded(bool& jump, bool isBig)
{
	frogState_ = WAITING;
	jump = false;
	delay_ = sdlutils().rand().nextInt(1000, 2500);
	lastUpdate_ = sdlutils().currRealTime();
	if (isBig) {
		if (secondPhase_) jumpsUntilNextTongue_--;
		createWaves();
		if (angry_) /*Volver a rana normal*/;
	}
	else {
		jumpsUntilNextTongue_--;
	}
	//Iniciar animaci�n Idle y ponerla en repeticii�n, si esta en segunda fase poner idle de segunda fase -> !MIRIAM EL REPEAT DE FRAMED IMAGE ESTA INVERTIDO!
	/*anim_->changeanim(&sdlutils().images().at("ranaidle"), 6, 4, 3, 24);
	anim_->repeat(false);*/
}

void FrogAttackManager::nextAttack()
{
	if(!secondPhase_ && attr_->getLife() <= attr_->getMaxHp() * 0.5) {
		//Launch second phase animation
		frogState_ = WAITING;
		//delay_  = duracion de la animacion de cambio de fase
		lastUpdate_ = sdlutils().currRealTime();
		secondPhase_ = true;
		return;
	}
	if (jumpsUntilNextTongue_ == 0) {
		jumpsUntilNextTongue_ = sdlutils().rand().nextInt(3, 5);
		frogState_ = TONGUE;
		//No tengo ni idea de como se lanzar� la animacion aqui
		if (!secondPhase_) createFly();
		tongueAttack_->attack(!secondPhase_? fly_->getComponent<Transform>() : player_);
		
	}
	else {
		int nextJump = secondPhase_ ? sdlutils().rand().nextInt(0, 100) : 100;
		if (nextJump >= 30) {
			//lanzar animacion de salto en el metodo attack
			frogJump_->attack(jumpDirection_);
			frogState_ = JUMPING;
		}
		else {
			bigJump_->attack(jumpDirection_);
			//lanzar animavion de salto en el metodo attack
			frogState_ = JUMPING_BIG;
		}

	}
}

