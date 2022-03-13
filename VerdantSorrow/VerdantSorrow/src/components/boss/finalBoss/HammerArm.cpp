#include "HammerArm.h"
#include "../../../ecs/Entity.h"
#include "../../Transform.h"
#include "../../RectangleCollider.h"
#include "../../../sdlutils/SDLUtils.h"
#include "../../../ecs/Manager.h"
#include "../frog_boss/wave/WaveMovement.h"
#include "../../RectangleRenderer.h"
#include "../../../game/CollisionManager.h"

HammerArm::HammerArm(CollisionManager* colManager) :colManager_(colManager), tr_(nullptr), state_(REPOSO), initialPos()
{
}

HammerArm::~HammerArm()
{
}

void HammerArm::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	collider_ = ent_->getComponent<RectangleCollider>();
	playertr_ = mngr_->getHandler(ecs::_PLAYER)->getComponent<Transform>();
	assert(tr_ != nullptr, collider_ != nullptr, playertr_ != nullptr);

	initialPos = Vector2D(tr_->getPos().getX(), tr_->getPos().getY());
}

void HammerArm::goDiagonal()
{
	collider_->setActive(false);

	if (abs(tr_->getPos().getX() - playerXPos) > 5 || abs(tr_->getPos().getY()) > 5) {
		Vector2D dir = Vector2D(playerXPos - tr_->getPos().getX(), - tr_->getPos().getY());/*initialPos - tr_->getPos();*/
		tr_->getVel().set(dir.normalize() * handSpeed);
	}
	else {
		tr_->getVel().set(Vector2D(0, 0));
		tr_->getPos().set(Vector2D(playerXPos, 0));
		changeState(HIT);
	}
}


void HammerArm::attack()
{

	collider_->setActive(true);
	collider_->setIsTrigger(true);

	if (tr_->getPos().getY() < sdlutils().height() - tr_->getHeight()) {
		tr_->getVel().set(Vector2D(0, handSpeed * 2));
	}
	else {
		tr_->getVel().set(Vector2D(0, 0));
		tr_->getPos().setY(sdlutils().height() - tr_->getHeight());
		lastTimeFloor = sdlutils().currRealTime();
		changeState(REPOSOSUELO);
		createWave(1);
		createWave(-1);
	}
}

void HammerArm::goBack()
{
	if (abs(tr_->getPos().getX() - initialPos.getX()) > 5 || abs(tr_->getPos().getY() - initialPos.getY()) > 5) {
		Vector2D dir = initialPos - tr_->getPos();
		tr_->getVel().set(dir.normalize() * handSpeed);
	}
	else {
		tr_->getVel().set(Vector2D(0, 0));
		tr_->getPos().set(initialPos);
		changeState(FIN);
	}
}

void HammerArm::stayFloor() {
	collider_->setIsTrigger(false);
	if (sdlutils().currRealTime() > lastTimeFloor + cooldoownInFloor)
		changeState(BACK);
}

void HammerArm::getPlayerX()
{
	playerXPos = playertr_->getPos().getX();
}

void HammerArm::createWave(int dir)
{
	//Se crea la onda expansiva
	auto Wave = mngr_->addEntity();
	//Se anyade el transform
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
	//Se le anyade un color inicial a la onda
	Wave->addComponent<RectangleRenderer>(SDL_Color());

	//Se anyade un collider a la onda
	auto waveCollider = Wave->addComponent<RectangleCollider>(WaveTr->getWidth(), WaveTr->getHeight());
	waveCollider->setIsTrigger(true);
	//Se anyade el collider al colliderGameManager
	colManager_->addCollider(waveCollider);
	//Se anyade el movimiento horizontal
	Wave->addComponent<WaveMovement>(WaveDir, WaveSpeed);
}
