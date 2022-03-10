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


HandsManager::HandsManager(CollisionManager* colManager):colmanager_(colManager)
{


	
}

HandsManager::~HandsManager()
{
}

void HandsManager::initComponent()
{
	leftHand_ = mngr_->addEntity();
	rightHand_ = mngr_->addEntity();

	tr_=ent_->getComponent<Transform>();
	auto manoIzTr = leftHand_->addComponent<Transform>();
	manoIzTr->init(tr_->getPos() + Vector2D(-100, 100), Vector2D(), 50, 50, 0.0f, false);
	leftHand_->addComponent<Image>(&sdlutils().images().at("mano"));

	auto manoIzCollider = leftHand_->addComponent<RectangleCollider>(manoIzTr->getWidth(), manoIzTr->getHeight());
	colmanager_->addCollider(manoIzCollider);
	leftHand_->addComponent<ClapAttack>(true);
	/*leftHand_->addComponent<Punietazo>();*/


	auto manoDrTr = rightHand_->addComponent<Transform>();
	manoDrTr->init(tr_->getPos() + Vector2D(100 +tr_->getWidth() - 50, 100), Vector2D(), 50, 50, 0.0f, false);
	rightHand_->addComponent<Image>(&sdlutils().images().at("mano"));


	

	//Se añade un collider a la rana
	auto manoDrCollider = rightHand_->addComponent<RectangleCollider>(manoDrTr->getWidth(), manoDrTr->getHeight());
	colmanager_->addCollider(manoDrCollider);
	rightHand_->addComponent<ClapAttack>(false);
//	rightHand_->addComponent<Punietazo>();
}
