#include "HandsManager.h"
#include "../../../ecs/Entity.h"
#include "../../../ecs/Manager.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../Transform.h"
#include "../../RectangleCollider.h"
#include "../../Image.h"
#include "../../FramedImage.h"
#include "../CollideWithBordersBoss.h"
#include "Punietazo.h"
#include "ClapAttack.h"
#include "HammerArm.h"
#include "../BossAtributos.h"
#include "../../fondos/ParticleSystem.h"


HandsManager::HandsManager(CollisionManager* colManager) :colmanager_(colManager), multFase_(1), state_(REPOSO),lastAttackDone_(),tiempoColor_()
{
}

HandsManager::~HandsManager()
{
}

void HandsManager::initComponent()
{
	bA_ = ent_->getComponent<BossAtributos>();
	playertr_ = mngr_->getHandler(ecs::_PLAYER)->getComponent<Transform>();
	assert(playertr_ != nullptr && bA_ != nullptr);
	lastAttackDone_ = mngr_->addTimer();
	//tiempoColor_.reset();
	tiempoColor_ =mngr_->addTimer();
	createHands();
	chooseAttack();

}

void HandsManager::update()
{
	if (state_ == REPOSO && lastAttackDone_->currTime() > attackCooldown) {
		switch (numeroAtaque)
		{
		case CLAP:
			state_ = CLAP;
			break;
		case PUNIETAZO:
			state_ = PUNIETAZO;
			break;
		case MARTILLAZO:
			state_ = MARTILLAZO;
			break;
		default:
			break;
		}
	}

	if (state_ == CLAP)
		clapAttack();
	else if (state_ == PUNIETAZO) // primero el derecho luego el izdo
		punietazoAttack();
	else if (state_ == MARTILLAZO)
		hammerAttack();

	if (bA_->getLife() <= bA_->getMaxHp() / 2) {
		if (multFase_ == 1) {
			multFase_ = 4;
		}
		else
		{
			colliderLeftHand_->setIsTrigger(true);
			colliderRightHand_->setIsTrigger(true);

			if (tiempoColor_->currTime() >= 500) {
				rightHandImg_->setColor(200, 20, 200);
				leftHandImg_->setColor(200, 20, 200);
			}

			ParticleSystem* particlesys = new ParticleSystem(&sdlutils().images().at("luz_morado"), mngr_);
			particlesys->createParticlesFire(2, leftHandTr_->getPos().getX() + sdlutils().rand().nextInt(0, (leftHandTr_->getWidth())), leftHandTr_->getPos().getY() + 20);
		
			ParticleSystem* particlesys3 = new ParticleSystem(&sdlutils().images().at("luz_morado"), mngr_);
			particlesys->createParticlesFire(2, rightHandTr_->getPos().getX() + sdlutils().rand().nextInt(0, (rightHandTr_->getWidth())), rightHandTr_->getPos().getY() + 20);}
	}
	
}

void HandsManager::createHands() {
	float handSize = 200;
	float width_colliderOffset = 40;
	float height_colliderOffset = 60;

	leftHand_ = mngr_->addEntity();
	rightHand_ = mngr_->addEntity();

	tr_ = ent_->getComponent<Transform>();
	leftHandTr_ = leftHand_->addComponent<Transform>();
	leftHandTr_->init(tr_->getPos() + Vector2D(150 + tr_->getWidth(), 150), Vector2D(), handSize, handSize, 0.0f, false);
	leftHandImg_ = leftHand_->addComponent<Image>(&sdlutils().images().at("manoIzq"));

	auto manoIzCollider = leftHand_->addComponent<RectangleCollider>
		(leftHandTr_->getWidth() - width_colliderOffset, leftHandTr_->getHeight() - height_colliderOffset);
	colmanager_->addCollider(manoIzCollider);
	clapLeft_ = leftHand_->addComponent<ClapAttack>(true);
	punietazoleft_ = leftHand_->addComponent<Punietazo>(false);
	hammerLeft_ = leftHand_->addComponent<HammerArm>(colmanager_);


	rightHandTr_ = rightHand_->addComponent<Transform>();
	rightHandTr_->init(tr_->getPos() + Vector2D(- handSize - 150, 150), Vector2D(), handSize, handSize, 0.0f, false);
	rightHandImg_ = rightHand_->addComponent<Image>(&sdlutils().images().at("manoDer"));

	auto manoDrCollider = rightHand_->addComponent<RectangleCollider>
		(rightHandTr_->getWidth() - width_colliderOffset, rightHandTr_->getHeight() - height_colliderOffset);
	colmanager_->addCollider(manoDrCollider);
	clapRight_ = rightHand_->addComponent<ClapAttack>(false);
	punietazoright_ = rightHand_->addComponent<Punietazo>(true);
	hammerRight_ = rightHand_->addComponent<HammerArm>(colmanager_);

	colliderLeftHand_ = manoIzCollider;
	colliderRightHand_ = manoDrCollider;
	assert(colliderLeftHand_ != nullptr && colliderRightHand_ != nullptr);
	leftHand_->addToGroup(ecs::_BOSS_GRP);
	rightHand_->addToGroup(ecs::_BOSS_GRP);
}

void HandsManager::chooseAttack() {

	int punietazoProb = 2;
	int martillazoProb = 6;

	if (state_ == CLAP) {
		attackCooldown = sdlutils().rand().nextInt(1500, 2501) * multFase_;
		auto ataqueElegido = sdlutils().rand().nextInt(0, 10);
		if (ataqueElegido <= punietazoProb) numeroAtaque = PUNIETAZO;
		else if (ataqueElegido <= martillazoProb) numeroAtaque = MARTILLAZO;
		else numeroAtaque = CLAP;
	}
	else if (state_ == PUNIETAZO) {
		attackCooldown = sdlutils().rand().nextInt(250, 501);
		auto ataqueElegido = sdlutils().rand().nextInt(0, 10);
		if (ataqueElegido <= punietazoProb) numeroAtaque = PUNIETAZO;
		else if (ataqueElegido <= martillazoProb) numeroAtaque = MARTILLAZO;
		else numeroAtaque = CLAP;
	}
	else if(state_ == MARTILLAZO){
		attackCooldown = sdlutils().rand().nextInt(1000, 2001);
		auto ataqueElegido = sdlutils().rand().nextInt(0, 10);
		if (ataqueElegido <= punietazoProb) numeroAtaque = PUNIETAZO;
		else if (ataqueElegido <= martillazoProb) numeroAtaque = MARTILLAZO;
		else numeroAtaque = CLAP;
	}
	else {
		attackCooldown = 1000;
		//lastAttackDone = sdlutils().currRealTime();
		lastAttackDone_->reset();
		auto ataqueElegido = sdlutils().rand().nextInt(0, 10);
		if (ataqueElegido <= punietazoProb) numeroAtaque = PUNIETAZO;
		else if (ataqueElegido <= martillazoProb) numeroAtaque = MARTILLAZO;
		else numeroAtaque = CLAP;
	}
}

void HandsManager::clapAttack(){
	if (clapLeft_->getstate() == ClapAttack::REPOSO || clapRight_->getstate() == ClapAttack::REPOSO) {
		clapLeft_->changeState(ClapAttack::DIAGONAL);
		clapRight_->changeState(ClapAttack::DIAGONAL); 
		leftHandImg_->setColor(200, 200, 20, 500);
		rightHandImg_->setColor(200, 200, 20, 500);
		
		tiempoColor_->reset();

		ParticleSystem* particlesys = new ParticleSystem(&sdlutils().images().at("luz_rosa"), mngr_);
		particlesys->createParticlesHandMagic(10, leftHandTr_);
		ParticleSystem* particlesys2 = new ParticleSystem(&sdlutils().images().at("luz_rosa"), mngr_);
		particlesys2->createParticlesHandMagic(10, rightHandTr_);
	}
	else if (clapLeft_->getstate() == ClapAttack::DIAGONAL || clapRight_->getstate() == ClapAttack::DIAGONAL) {
		clapLeft_->goDiagonal();
		clapRight_->goDiagonal();
	}
	else if (clapLeft_->getstate() == ClapAttack::CENTER || clapRight_->getstate() == ClapAttack::CENTER) {
		
		if (multFase_ == 1) {
			clapLeft_->goCenter(false);
			clapRight_->goCenter(false);
		}
		else
		{
			clapLeft_->goCenter(true);
			clapRight_->goCenter(true);
		}
	}
	else if (clapLeft_->getstate() == ClapAttack::REPOSOSUELO || clapRight_->getstate() == ClapAttack::REPOSOSUELO) {
		clapLeft_->stayFloor();
		clapRight_->stayFloor();
	}
	else if (clapLeft_->getstate() == ClapAttack::BACK || clapRight_->getstate() == ClapAttack::BACK) {
		clapLeft_->goBack();
		clapRight_->goBack();
	}
	else if (clapLeft_->getstate() == ClapAttack::FIN && clapRight_->getstate() == ClapAttack::FIN) {
		clapLeft_->changeState(ClapAttack::REPOSO);
		clapRight_->changeState(ClapAttack::REPOSO);

		chooseAttack();
		//lastAttackDone = sdlutils().currRealTime();
		lastAttackDone_->reset();
		state_ = REPOSO;
	}
}

void HandsManager::punietazoAttack() {
	if (punietazoright_->getstate() == Punietazo::REPOSO) {
		punietazoright_->changeState(Punietazo::DOWN);

		ParticleSystem* particlesys = new ParticleSystem(&sdlutils().images().at("luz_rosa"), mngr_);
		particlesys->createParticlesHandMagic(20, rightHandTr_);
	}

	else if (punietazoright_->getstate() == Punietazo::DOWN)
		punietazoright_->goDown();

	else if (punietazoright_->getstate() == Punietazo::FOLLOW)
		punietazoright_->followPlayer();

	else if (punietazoright_->getstate() == Punietazo::HIT)
		punietazoright_->hit();

	else if (punietazoright_->getstate() == Punietazo::BACK)
		punietazoright_->goBack();



	else if (punietazoleft_->getstate() == Punietazo::REPOSO) {
		punietazoleft_->changeState(Punietazo::DOWN);

		ParticleSystem* particlesys = new ParticleSystem(&sdlutils().images().at("luz_rosa"), mngr_);
		particlesys->createParticlesHandMagic(20, leftHandTr_);
	}

	else if (punietazoleft_->getstate() == Punietazo::DOWN)
		punietazoleft_->goDown();

	else if (punietazoleft_->getstate() == Punietazo::FOLLOW)
		punietazoleft_->followPlayer();

	else if (punietazoleft_->getstate() == Punietazo::HIT)
		punietazoleft_->hit();

	else if (punietazoleft_->getstate() == Punietazo::BACK)
		punietazoleft_->goBack();


	else {
		punietazoleft_->changeState(Punietazo::REPOSO);
		punietazoright_->changeState(Punietazo::REPOSO);

		chooseAttack();
		//lastAttackDone = sdlutils().currRealTime();
		lastAttackDone_->reset();
		state_ = REPOSO;
	}
}

void HandsManager::hammerAttack() {
	if (hammerLeft_->getstate() == HammerArm::REPOSO && hammerRight_->getstate() == HammerArm::REPOSO) {
		if (playertr_->getPos().getX() - playertr_->getWidth() < sdlutils().width() / 2) {
			hammerRight_->changeState(HammerArm::DIAGONAL);
			hammerRight_->getPlayerX();
		}
		else {
			hammerLeft_->changeState(HammerArm::DIAGONAL);
			hammerLeft_->getPlayerX();
		}
	}
	else if (hammerLeft_->getstate() == HammerArm::REPOSO) {
		if (hammerRight_->getstate() == HammerArm::DIAGONAL) {
			rightHandImg_->setColor(200, 200, 20, 500);
			tiempoColor_->reset();
			hammerRight_->goDiagonal();

			ParticleSystem* particlesys = new ParticleSystem(&sdlutils().images().at("luz_rosa"), mngr_);
			particlesys->createParticlesHandMagic(1, rightHandTr_);
		}
		else if (hammerRight_->getstate() == HammerArm::HIT) {
			if(multFase_== 1)
				hammerRight_->attack(false);
			else 
				hammerRight_->attack(true);
		}
		else if (hammerRight_->getstate() == HammerArm::REPOSOSUELO) {
			hammerRight_->stayFloor();
		}
		else if (hammerRight_->getstate() == HammerArm::BACK) {
			hammerRight_->goBack();
		}
		else if (hammerRight_->getstate() == HammerArm::FIN) {
			hammerRight_->changeState(HammerArm::REPOSO);

			chooseAttack();
			//lastAttackDone = sdlutils().currRealTime();
			lastAttackDone_->reset();
			state_ = REPOSO;
		}
	}
	else {
		if (hammerRight_->getstate() == HammerArm::REPOSO) {
			if (hammerLeft_->getstate() == HammerArm::DIAGONAL) {
				hammerLeft_->goDiagonal();
				leftHandImg_->setColor(200, 200, 20, 500);
				
				tiempoColor_->reset();

				ParticleSystem* particlesys = new ParticleSystem(&sdlutils().images().at("luz_rosa"), mngr_);
				particlesys->createParticlesHandMagic(1, leftHandTr_);
			}
			else if (hammerLeft_->getstate() == HammerArm::HIT) {
				
				if (multFase_ == 1)
					hammerLeft_->attack(false);
				else
					hammerLeft_->attack(true);
			}
			else if (hammerLeft_->getstate() == HammerArm::REPOSOSUELO) {
				hammerLeft_->stayFloor();
			}
			else if (hammerLeft_->getstate() == HammerArm::BACK) {
				hammerLeft_->goBack();
			}
			else if (hammerLeft_->getstate() == HammerArm::FIN) {
				hammerLeft_->changeState(HammerArm::REPOSO);

				chooseAttack();
				//lastAttackDone = sdlutils().currRealTime();
				lastAttackDone_->reset();
				state_ = REPOSO;
			}
		}
	}
}

