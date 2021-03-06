#include "FlyMovement.h"
#include "../../Transform.h"
#include "../../../ecs/Entity.h"
#include "../../../ecs/Manager.h"
#include "../../../sdlutils/SoundEffect.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../../game/Hub.h"

FlyMovement::FlyMovement() : tr_(), dirX_(1), dirY_(1), playerTr_(), spaceWhereExplotes(5), timeOfLife_(2000)
{
}

FlyMovement::~FlyMovement()
{
}

void FlyMovement::initComponent() {
	tr_ = ent_->getComponent<Transform>();
	playerTr_ = mngr_->getHandler(ecs::_PLAYER)->getComponent<Transform>();

	SoundEffect* s = &sdlutils().soundEffects().at("sfx_mosca_idle");
	s->play();
	s->setChannelVolume(*sC().getHubScene()->getMusicVolume());

	bool correct = tr_ != nullptr && playerTr_ != nullptr;
	vt_ = mngr_->addTimer();
	vt_->reset();
}

void FlyMovement::update()
{
	auto& vel = tr_->getVel();

	//Calcula velocidad y le da velocidad a la mosca
	dirX_ = (playerTr_->getPos().getX() - tr_->getPos().getX()) / abs(playerTr_->getPos().getX() - tr_->getPos().getX());
	dirY_ = (playerTr_->getPos().getY() - tr_->getPos().getY()) / abs(playerTr_->getPos().getY() - tr_->getPos().getY());
	//4 excepciones porque siempre es speed hasta que de repente es -20045096 y se va a parla
	if (dirY_ < -SPEED) dirY_ = -SPEED;
	else if (dirY_ > SPEED) dirY_ = SPEED;
	if (dirX_ < -SPEED) dirX_ = -SPEED;
	else if (dirX_ > SPEED) dirX_ = SPEED;

	vel = new Vector2D(dirX_ * SPEED, dirY_ * SPEED);;
	
	//Si queda poco espacio explota y muere
	if (vt_->currTime() > timeOfLife_) {
		ent_->setAlive(false);
		vt_->reset();
	}
}