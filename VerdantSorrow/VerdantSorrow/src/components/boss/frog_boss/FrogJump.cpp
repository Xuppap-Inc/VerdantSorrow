#include "FrogJump.h"

#include "../../Transform.h"
#include "../../FramedImage.h"
#include "../../../ecs/Entity.h"

FrogJump::FrogJump(): tr_(), isJumping_(false), jumpForce_(), lastJump_(), jumpCd_(2000), fr_()
{
}

FrogJump::FrogJump(float jumpForce): tr_(), isJumping_(false), jumpForce_(jumpForce), lastJump_(), jumpCd_(1000), fr_()
{
}

void FrogJump::initComponent()
{
	fr_ = ent_->getComponent<FramedImage>();
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
}

/*
* Metodo que salta en la direccion especificada una distancia aleatoria
* @param dir: dirección del salto (-1: izquierda, 1: derecha)
*/
void FrogJump::attack(int dir) {

	auto& RNG = sdlutils().rand();
	auto& vel = tr_->getVel();

	//x = {-1, 1} 
	int x = RNG.nextInt(500, 1000)/100.0 * dir;
	vel = new Vector2D(x, -jumpForce_);
	vel = vel.normalize() * jumpForce_;

	SoundEffect* s = &sdlutils().soundEffects().at("sfx_rana_jump");
	s->play();
}
