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


HandsManager::HandsManager(CollisionManager* colManager) :colmanager_(colManager), state_(REPOSO)
{

}

HandsManager::~HandsManager()
{
}

void HandsManager::initComponent()
{
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




	auto manoDrTr = rightHand_->addComponent<Transform>();
	manoDrTr->init(tr_->getPos() + Vector2D(-200 - handSize, 100), Vector2D(), handSize, handSize, 0.0f, false);
	rightHand_->addComponent<Image>(&sdlutils().images().at("mano"));

	//Se añade un collider a la rana
	auto manoDrCollider = rightHand_->addComponent<RectangleCollider>(manoDrTr->getWidth(), manoDrTr->getHeight());
	colmanager_->addCollider(manoDrCollider);
	clapRight_ = rightHand_->addComponent<ClapAttack>(false);
	punietazoright_ = rightHand_->addComponent<Punietazo>();


	state_ = PUNIETAZO;
}

void HandsManager::update()
{
	if (state_ == CLAP) {

		if (clapLeft_->getstate() == ClapAttack::REPOSO && clapRight_->getstate() == ClapAttack::REPOSO) {
			clapLeft_->changeState(ClapAttack::SIDE);
			clapRight_->changeState(ClapAttack::SIDE);
		}
		else if (clapLeft_->getstate() == ClapAttack::SIDE || clapRight_->getstate() == ClapAttack::SIDE) {
			clapLeft_->goSide();
			clapRight_->goSide();
		}
		else if (clapLeft_->getstate() == ClapAttack::DOWN || clapRight_->getstate() == ClapAttack::DOWN) {
			clapLeft_->goDown();
			clapRight_->goDown();
		}
		else if (clapLeft_->getstate() == ClapAttack::CENTER || clapRight_->getstate() == ClapAttack::CENTER) {
			clapLeft_->goCenter();
			clapRight_->goCenter();
		}
		else if (clapLeft_->getstate() == ClapAttack::BACK || clapRight_->getstate() == ClapAttack::BACK) {
			clapLeft_->goBack();
			clapRight_->goBack();
		}
		else if (clapLeft_->getstate() == ClapAttack::FIN && clapRight_->getstate() == ClapAttack::FIN) {
			clapLeft_->changeState(ClapAttack::REPOSO);
			clapRight_->changeState(ClapAttack::REPOSO);

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
			punietazoleft_->changeState(Punietazo::REPOSO);

			state_ = REPOSO;
		}

	}
	else if (state_ == MARTILLAZO) {

	}

}
