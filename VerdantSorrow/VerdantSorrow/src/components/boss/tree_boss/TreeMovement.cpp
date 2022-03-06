#include "TreeMovement.h"
#include "../../../components/Transform.h"
#include "../../../ecs/Entity.h"

TreeMovement::TreeMovement() :
	tr_(),
	playerTr_(),
	offsetX_()
{
}
TreeMovement::TreeMovement(Transform* playerTransform_, float offsetX, float followVelocity) :
	tr_(),
	playerTr_(playerTransform_),
	offsetX_(offsetX),
	followVel_(followVelocity)
{
}
TreeMovement::~TreeMovement() {}

void TreeMovement::initComponent()
{
	tr_ = ent_->getComponent<Transform>(); //Guarda el transform de la mosca
	assert(tr_ != nullptr);
	assert(playerTr_ != nullptr);
}

void TreeMovement::update()
{
	auto playerPos = playerTr_->getPos();
	auto playerVel = playerTr_->getVel();
	auto& treePos = tr_->getPos();
	auto& treeVel = tr_->getVel();
	Vector2D distance;

	//Si se encuentra el jugador en el cuerpo de groot no se mueve
	if (playerPos.getX() > tr_->getPos().getX() && playerPos.getX() < tr_->getPos().getX() + tr_->getWidth())
		treeVel = Vector2D(0, 0);
	else {
		//Se calcula la velocidad sumando o restando el ancho del jugador,
		//Dependiendo de si se encuentra en la derecha o en la izquierda del árbol
		if (playerPos.getX() < tr_->getPos().getX()) distance = (playerPos + Vector2D(playerTr_->getWidth(), 0) - treePos);
		else distance = (playerPos - Vector2D(playerTr_->getWidth(), 0) - treePos);

		//Calcula la velocidad, y según un offset elegido a mano, si está demasiado cerca se parará
		auto velX = distance.normalize().getX();
		if (abs(distance.getX()) <= offsetX_)
		{
			velX = 0.0f;
		}
		treeVel = Vector2D((velX * followVel_), 0);
	}
	
}