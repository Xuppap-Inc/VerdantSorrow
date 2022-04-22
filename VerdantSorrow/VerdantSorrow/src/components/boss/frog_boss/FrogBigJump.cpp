#include "FrogBigJump.h"

#include "../../Transform.h"
#include "../../../ecs/Entity.h"

FrogBigJump::FrogBigJump() : tr_(), isJumping_(false), jumpForce_(), lastJump_(), jumpCd_(2000)
{
}

FrogBigJump::FrogBigJump(float jumpForce) : tr_(), isJumping_(false), jumpForce_(jumpForce), lastJump_(), jumpCd_(2000)
{
}

void FrogBigJump::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
}

/*
* Metodo que salta en la direccion especificada una distancia aleatoria
* @param dir: dirección del salto (-1: izquierda, 1: derecha)
*/
void FrogBigJump::attack(int dir) {

	auto& vel = tr_->getVel();

	//x = {-1, 0, 1} 
	int x = 10 * dir;
	vel = new Vector2D(x, -jumpForce_);
	vel = vel.normalize() * jumpForce_;
}