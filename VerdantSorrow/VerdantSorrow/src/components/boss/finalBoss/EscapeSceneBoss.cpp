#include "EscapeSceneBoss.h"
#include "FinalBossMovement.h"
#include "../../../ecs/Entity.h"
#include "../../../ecs/Manager.h"
#include "../../Transform.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../boss/BossAtributos.h"
#include "../../Image.h"
#include "../../../game/CollisionManager.h"
#include "../../FramedImage.h"
#include "../../../sdlutils/VirtualTimer.h"
#include "../../../game/Hub.h"

EscapeSceneBoss::EscapeSceneBoss(CollisionManager* colManager) :
	tr_(nullptr), colManager_(colManager), 
	eyeSpeed_(3), deadBoss_(false)
{
}
EscapeSceneBoss::~EscapeSceneBoss()
{
}

void EscapeSceneBoss::initComponent()
{

	tr_ = ent_->getComponent<Transform>();

	assert(tr_ != nullptr, bA_ != nullptr, waveSp_ != nullptr);



}

void EscapeSceneBoss::update()
{
	if (active_) {
		bounce();
	}

}

void EscapeSceneBoss::bounce()
{
	auto& pos_ = tr_->getPos();
	auto& vel_ = tr_->getVel();

	if (vel_.magnitude() == 0)
		vel_ = Vector2D(1, 1);

	auto cameraTr = mngr_->getHandler(ecs::_hdlr_CAMERA)->getComponent<Transform>();
	//tr_->getVel().set(cameraTr->getPos().getX(), cameraTr->getPos().getY());
	// bounce on top/bottom borders
	if (pos_.getY() < cameraTr->getPos().getY() + cameraTr->getHeight() / 2) {

		pos_.setY(cameraTr->getPos().getY() + cameraTr->getHeight() / 2);
		vel_.setY(-vel_.getY());
	}
	else if (pos_.getY() + tr_->getHeight() > cameraTr->getPos().getY() + sdlutils().height() - 100) {

		pos_.setY(cameraTr->getPos().getY() + sdlutils().height() - tr_->getHeight() - 100);
		vel_.setY(-vel_.getY());
	}
	else if (pos_.getX() + tr_->getWidth() > cameraTr->getPos().getX() + sdlutils().width()) {

		pos_.setX(cameraTr->getPos().getX() + sdlutils().width() - tr_->getWidth());
		vel_.setX(-vel_.getX());
	}
	else if (pos_.getX() < cameraTr->getPos().getX()) {

		pos_.setX(cameraTr->getPos().getX());
		vel_.setX(-vel_.getX());
	}

	if (vel_.magnitude() != 0)
		vel_ = vel_.normalize() * eyeSpeed_;
}

