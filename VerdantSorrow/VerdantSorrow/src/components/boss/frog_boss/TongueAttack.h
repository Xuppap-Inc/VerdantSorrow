#pragma once
#include "../../../ecs/Component.h"
#include "../../RectangleCollider.h"

#include "../../../sdlutils/VirtualTimer.h"

class Transform;
class CollisionManager;

class TongueAttack : public RectangleCollider
{
public :
	__CMPID_DECL__(ecs::_FROGTONGUE)
	TongueAttack(); //Constructor por defecto
	TongueAttack(CollisionManager * colMan);
	~TongueAttack();
	void initComponent() override;
	void update() override;
	void attack(bool fly); //metodo que lanza la lengua hacia el jugador Transform * objective
	bool finished();
	void cancel();
	void currentPos(bool fly);
protected:

	void setCollider(Vector2D pos, float w,float h); //crea el collider en la posicion que quiere
	CollisionManager* colMan_;
	float delay_;
	VirtualTimer* timer_;
	float lastUpdate_;
	bool finishedAttack_;
	bool fly_;

	Transform* frogTr_;
};

