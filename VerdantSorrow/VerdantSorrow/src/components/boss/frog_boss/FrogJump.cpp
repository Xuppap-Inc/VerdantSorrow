#include "FrogJump.h"

#include "../../Transform.h"
#include "../../FramedImage.h"
#include "../../../ecs/Entity.h"

FrogJump::FrogJump(): tr_(), isJumping_(false), jumpForce_(), lastJump_(), jumpCd_(2000)
{
}

FrogJump::FrogJump(float jumpForce): tr_(), isJumping_(false), jumpForce_(jumpForce), lastJump_(), jumpCd_(1000)
{
}

void FrogJump::initComponent()
{
	fr_ = ent_->getComponent<FramedImage>();
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
}


void FrogJump::update()
{
	//auto& RNG = sdlutils().rand();
	//auto& vel = tr_->getVel();

	//if (lastJump_ + jumpCd_ <= sdlutils().currRealTime() && !isJumping_) {
	//	isJumping_ = true;
	//	auto x = RNG.nextInt(-1000, 1000) / 100.0;
	///*	std::cout << x << std::endl;*/
	//	vel = new Vector2D(x, -jumpForce_);
	//	vel = vel.normalize() * jumpForce_;
	//	fr_->changeanim(&sdlutils().images().at("ranajump"), 6, 6, 500, 31);
	//	fr_->repeat(false);
	//	if (x < 0)fr_->flipX(false);
	//	else fr_->flipX(true);
	//}
	//	//flipx





	//if (isJumping_ && tr_->getPos().getY() >= sdlutils().height() - tr_->getHeight()) {
	//	isJumping_ = false;
	//	lastJump_ = sdlutils().currRealTime();
	//	

	//}
	
}

void FrogJump::attack(int dir) {
	//Habrá que esperar los frames de anticipación de la animación cuando haya
	auto& RNG = sdlutils().rand();
	auto& vel = tr_->getVel();

	//x = {-1, 1} 
	int x = RNG.nextInt(500, 1000)/100.0 * dir;
	vel = new Vector2D(x, -jumpForce_);
	vel = vel.normalize() * jumpForce_;
}
