#pragma once
#include "../../../ecs/Component.h"
#include "../../RectangleCollider.h"

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
	void attack(Transform * objective); //metodo que lanza la lengua hacia el jugador

protected:
	//Transform* tr_;
	void setCollider(Vector2D pos, float w,float h); //crea el collider en la posicion que quiere
	CollisionManager* colMan_;
	float delay_=100;
	float currentTime;
};

