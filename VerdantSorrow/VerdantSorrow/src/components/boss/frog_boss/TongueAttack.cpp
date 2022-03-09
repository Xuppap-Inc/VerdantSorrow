#include "TongueAttack.h"
#include "../../Transform.h"
#include "../../FramedImage.h"
#include "../../../ecs/Entity.h"
#include "../../../game/CollisionManager.h"
#include "../../../sdlutils/InputHandler.h"
#include "../../../ecs/Manager.h"
#include "../../RectangleRenderer.h"

TongueAttack::TongueAttack(CollisionManager *colMan):RectangleCollider(), 
	colMan_(colMan),delay_(1000), lastUpdate_(0), finishedAttack_(false), fly_(false)
{
	
	setIsTrigger(true);
	setActive(false);
	colMan->addCollider(this);
	
	
}
TongueAttack::TongueAttack() :RectangleCollider(),
	colMan_(nullptr), delay_(1000), lastUpdate_(0), finishedAttack_(false), fly_(false)
{
	setIsTrigger(true);
	setActive(false);
}
TongueAttack::~TongueAttack() {}

void TongueAttack::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
}
void TongueAttack::update()
{
	if (sdlutils().currRealTime() - lastUpdate_ >= delay_) //Desactiva la lengua cuando pasa un tiempo determinado
	{
		setActive(false);
		finishedAttack_ = true;
		lastUpdate_ = sdlutils().currRealTime(); //Actualiza al tiempo de juego actual
		
	}
	
}
void TongueAttack::attack(bool fly)
{
	fly_ = fly;
	//Compruebo si el objetivo es la mosca o el jugador 
	auto entObjective = fly ? mngr_->getHandler(ecs::_FLY) : mngr_->getHandler(ecs::_PLAYER); 
	auto objective = entObjective->getComponent<Transform>();
	auto posFrog = tr_->getPos(); //Posicion rana
	auto posObj = objective->getPos(); //Posicion mosca objetivo
	Vector2D iniPos; //Posicion inicial para el collider
	Vector2D finPos; //Posicion final para el collider
	float tongW; //Ancho de la lengua


	if (posObj.getX() < posFrog.getX()) //Si la mosca esta a su izq, el collider crece desde la mosca
	{
		iniPos = Vector2D(posObj.getX()+objective->getWidth(), posObj.getY());
		finPos = posFrog;

	}
	else if (posObj.getX() > posFrog.getX()) //En este caso el collider crece desde la rana
	{
		iniPos = Vector2D(posFrog.getX() + tr_->getWidth(), posFrog.getY());
		finPos=posObj;
	}

	tongW = (finPos - iniPos).magnitude();
	setCollider(iniPos,tongW,50); //Crea el collider y lo activa
	setActive(true);
	finishedAttack_ = false; //El ataque no ha terminado aun

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

void TongueAttack::setCollider(Vector2D pos, float w, float h)
{
	pos_ = pos;
	width_ = w;
	height_ = h;

}
