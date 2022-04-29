#include "TongueAttack.h"
#include "../../Transform.h"
#include "../../FramedImage.h"
#include "../../../ecs/Entity.h"
#include "../../../game/CollisionManager.h"
#include "../../../sdlutils/InputHandler.h"
#include "../../../ecs/Manager.h"
#include "../../RectangleRenderer.h"

TongueAttack::TongueAttack(CollisionManager* colMan) :RectangleCollider(),
colMan_(colMan), delay_(1000), lastUpdate_(0), finishedAttack_(false), fly_(false), frogTr_(), timer_()
{


	setIsTrigger(true);
	setActive(false);
	colMan->addCollider(this);
}

TongueAttack::TongueAttack() :RectangleCollider(),
colMan_(nullptr), delay_(1000), lastUpdate_(0), finishedAttack_(false), fly_(false), frogTr_(), timer_()
{
	

	setIsTrigger(true);
	setActive(false);
}
TongueAttack::~TongueAttack() {}

void TongueAttack::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
	frogTr_ = mngr_->getHandler(ecs::_FROGBOSS)->getComponent<Transform>();
	assert(frogTr_ != nullptr);
	timer_ = mngr_->addTimer();
}

void TongueAttack::update()
{
	if (!finishedAttack_ && timer_->currTime() >= delay_) //Desactiva la lengua cuando pasa un tiempo determinado
	{
		setActive(false);
		finishedAttack_ = true;
	}
}
void TongueAttack::attack(bool fly)
{
	currentPos(fly);
	setActive(true);

	finishedAttack_ = false; //El ataque no ha terminado aun
	timer_->reset();

	if (fly_)
	{
		mngr_->getHandler(ecs::_FLY)->setAlive(false); //Si el objetivo es la mosca, la mata
	}
}

bool TongueAttack::finished()
{
	return finishedAttack_;
}

void TongueAttack::cancel()
{
	setActive(false);
}

/*
* Metodo que posiciona la lengua dependiendo de la posicion actual de la rana y el objetivo de esta
* @param fly: booleana que indica si el objetivo es la mosca (true) o el jugador (false)
*/
void TongueAttack::currentPos(bool fly)
{
	fly_ = fly; //Compruebo si el objetivo es la mosca o el jugador 

	auto flyEnt_ = mngr_->getHandler(ecs::_FLY);
	auto playerEnt_ = mngr_->getHandler(ecs::_PLAYER);

	auto playerTr_ = playerEnt_->getComponent<Transform>();
	auto flyTr_ = flyEnt_->getComponent<Transform>();

	Transform* objective;
	if (fly) objective = flyTr_;
	else objective = playerTr_;

	auto posFrog = frogTr_->getPos(); //Posicion rana
	auto posObj = objective->getPos(); //Posicion mosca objetivo

	auto tongueXOffset = 70;
	auto tongueYOffset = 20;

	Vector2D iniPos; //Posicion inicial para el collider
	float w;
	if (posObj.getX() <= posFrog.getX()) //Si la mosca esta a su izq, el collider crece desde la mosca
	{
		iniPos = Vector2D(posObj.getX(), sdlutils().height() - playerTr_->getHeight() - GROUND_HEIGHT + tongueYOffset);
		w = posFrog.getX() + tongueXOffset - iniPos.getX();
	}

	else //En este caso el collider crece desde la rana
	{
		iniPos = Vector2D(posFrog.getX() - tongueXOffset + frogTr_->getWidth(), sdlutils().height() - playerTr_->getHeight() - GROUND_HEIGHT + tongueYOffset);
		w = posObj.getX() + objective->getWidth() - iniPos.getX();
	}

	tr_->setWidth(w);
	tr_->getPos().set(iniPos);
	setCollider(iniPos, w, tr_->getHeight());

}

void TongueAttack::setCollider(Vector2D pos, float w, float h)
{
	pos_ = pos;
	width_ = w;
	height_ = h;
}
