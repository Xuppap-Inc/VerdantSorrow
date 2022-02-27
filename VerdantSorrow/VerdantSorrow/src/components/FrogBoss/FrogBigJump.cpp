#include "FrogBigJump.h"

#include "../Transform.h"
#include "../../ecs/Entity.h"

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


void FrogBigJump::update()
{
	auto& RNG = sdlutils().rand();
	auto& vel = tr_->getVel();

	if (lastJump_ + jumpCd_ <= sdlutils().currRealTime() && !isJumping_) {
		isJumping_ = true;
		int x = 0;
		/*	std::cout << x << std::endl;*/
		vel = new Vector2D(x, -jumpForce_);
		vel = vel.normalize() * jumpForce_;
		//fr_->changeanim(&sdlutils().images().at("ranajump"), 6, 6, 2000, 31);
	}

	if (isJumping_ && tr_->getPos().getY() >= sdlutils().height() - tr_->getHeight()) {
		isJumping_ = false;
		lastJump_ = sdlutils().currRealTime();
	};
}

void FrogBigJump::attack(int dir) {
	//Habrá que esperar los frames de anticipación de la animación cuando haya
	auto& vel = tr_->getVel();

	//x = {-1, 0, 1} 
	int x = 10 * dir;
	vel = new Vector2D(x, -jumpForce_);
	vel = vel.normalize() * jumpForce_;
}