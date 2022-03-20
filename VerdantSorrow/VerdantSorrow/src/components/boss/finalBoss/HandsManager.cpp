#include "HandsManager.h"
#include "../../../ecs/Entity.h"
#include "../../../ecs/Manager.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../Transform.h"
#include "../../RectangleCollider.h"
#include "../../Image.h"
#include "../CollideWithBordersBoss.h"
#include "Punietazo.h"
#include "ClapAttack.h"
#include "HammerArm.h"
#include "../BossAtributos.h"


HandsManager::HandsManager(CollisionManager* colManager) :colmanager_(colManager), state_(REPOSO)
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

	float handSize = 100;

	leftHand_ = mngr_->addEntity();
	rightHand_ = mngr_->addEntity();

	tr_ = ent_->getComponent<Transform>();
	auto manoIzTr = leftHand_->addComponent<Transform>();
	manoIzTr->init(tr_->getPos() + Vector2D(200 + tr_->getWidth(), 100), Vector2D(), handSize, handSize, 0.0f, false);
	leftHand_->addComponent<Image>(&sdlutils().images().at("mano"));

	auto manoIzCollider = leftHand_->addComponent<RectangleCollider>(manoIzTr->getWidth(), manoIzTr->getHeight());
	colmanager_->addCollider(manoIzCollider);
	clapLeft_ = leftHand_->addComponent<ClapAttack>(true);
	punietazoleft_ = leftHand_->addComponent<Punietazo>();
	hammerLeft_ = leftHand_->addComponent<HammerArm>(colmanager_);



	auto manoDrTr = rightHand_->addComponent<Transform>();
	manoDrTr->init(tr_->getPos() + Vector2D(-200 - handSize, 100), Vector2D(), handSize, handSize, 0.0f, false);
	rightHand_->addComponent<Image>(&sdlutils().images().at("mano"));

	//Se añade un collider a la rana
	auto manoDrCollider = rightHand_->addComponent<RectangleCollider>(manoDrTr->getWidth(), manoDrTr->getHeight());
	colmanager_->addCollider(manoDrCollider);
	clapRight_ = rightHand_->addComponent<ClapAttack>(false);
	punietazoright_ = rightHand_->addComponent<Punietazo>();
	hammerRight_ = rightHand_->addComponent<HammerArm>(colmanager_);

	colliderLeftHand_ = manoIzCollider;
	colliderRightHand_ = manoDrCollider;
	assert(colliderLeftHand_ != nullptr && colliderRightHand_ != nullptr);

	state_ = CLAP;
}

void HandsManager::update()
{
	if (state_ == REPOSO && sdlutils().currRealTime() > lastAttackDone + attackCooldown) {
		auto numeroAtaque =  sdlutils().rand().nextInt(0, 3);
		switch (numeroAtaque)
		{
		case 0:
			state_ = CLAP;
			break;
		case 1:
			state_ = PUNIETAZO;
			break;
		case 2:
			state_ = MARTILLAZO;
			break;
		default:
			break;
		}
	}

	if (state_ == CLAP) {
		if (clapLeft_->getstate() == ClapAttack::REPOSO || clapRight_->getstate() == ClapAttack::REPOSO) {
			clapLeft_->changeState(ClapAttack::DIAGONAL);
			clapRight_->changeState(ClapAttack::DIAGONAL);
		}
		else if (clapLeft_->getstate() == ClapAttack::DIAGONAL || clapRight_->getstate() == ClapAttack::DIAGONAL) {
			clapLeft_->goDiagonal();
			clapRight_->goDiagonal();
		}
		else if (clapLeft_->getstate() == ClapAttack::CENTER || clapRight_->getstate() == ClapAttack::CENTER) {
			clapLeft_->goCenter();
			clapRight_->goCenter();
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

			attackCooldown = sdlutils().rand().nextInt(1000, 2001);
			lastAttackDone = sdlutils().currRealTime();
			state_ = REPOSO;
		}
	}
	else if (state_ == PUNIETAZO) { // primero el derecho luego el izdo

		if (punietazoright_->getstate() == Punietazo::REPOSO)
			punietazoright_->changeState(Punietazo::DOWN);

		else if (punietazoright_->getstate() == Punietazo::DOWN)
			punietazoright_->goDown();

		else if (punietazoright_->getstate() == Punietazo::FOLLOW)
			punietazoright_->followPlayer();

		else if (punietazoright_->getstate() == Punietazo::HIT)
			punietazoright_->hit();

		else if (punietazoright_->getstate() == Punietazo::BACK)
			punietazoright_->goBack();



		else if (punietazoleft_->getstate() == Punietazo::REPOSO)
			punietazoleft_->changeState(Punietazo::DOWN);

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

			attackCooldown = sdlutils().rand().nextInt(1000, 2001);
			lastAttackDone = sdlutils().currRealTime();
			state_ = REPOSO;
		}

	}
	else if (state_ == MARTILLAZO) {
		if (hammerLeft_->getstate() == HammerArm::REPOSO && hammerRight_->getstate() == HammerArm::REPOSO) {
			if (playertr_->getPos().getX() - playertr_->getWidth() < sdlutils().width() / 2){
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
				hammerRight_->goDiagonal();
			}
			else if (hammerRight_->getstate() == HammerArm::HIT) {
				hammerRight_->attack();
			}
			else if (hammerRight_->getstate() == HammerArm::REPOSOSUELO) {
				hammerRight_->stayFloor();
			}
			else if (hammerRight_->getstate() == HammerArm::BACK) {
				hammerRight_->goBack();
			}
			else if (hammerRight_->getstate() == HammerArm::FIN) {
				hammerRight_->changeState(HammerArm::REPOSO);

				attackCooldown = sdlutils().rand().nextInt(1000, 2001);
				lastAttackDone = sdlutils().currRealTime();
				state_ = REPOSO;
			}
		}
		else {
			if (hammerRight_->getstate() == HammerArm::REPOSO) {
				if (hammerLeft_->getstate() == HammerArm::DIAGONAL) {
					hammerLeft_->goDiagonal();
				}
				else if (hammerLeft_->getstate() == HammerArm::HIT) {
					hammerLeft_->attack();
				}
				else if (hammerLeft_->getstate() == HammerArm::REPOSOSUELO) {
					hammerLeft_->stayFloor();
				}
				else if (hammerLeft_->getstate() == HammerArm::BACK) {
					hammerLeft_->goBack();
				}
				else if (hammerLeft_->getstate() == HammerArm::FIN) {
					hammerLeft_->changeState(HammerArm::REPOSO);

					attackCooldown = sdlutils().rand().nextInt(1000, 2001);
					lastAttackDone = sdlutils().currRealTime();
					state_ = REPOSO;
				}
			}
		}
	}
	if (bA_->getLife() <= bA_->getMaxHp() / 2) {
		colliderLeftHand_->setIsTrigger(true);
		colliderRightHand_->setIsTrigger(true);
	}
}
