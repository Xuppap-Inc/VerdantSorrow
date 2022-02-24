#include "FrogBigJumpLateral.h"

#include "../Transform.h"
#include "../../ecs/Entity.h"

FrogBigJumpLateral::FrogBigJumpLateral() : tr_(), isJumping_(false), jumpForce_(), lastJump_(), jumpCd_(2000)
{
}

FrogBigJumpLateral::FrogBigJumpLateral(float jumpForce) : tr_(), isJumping_(false), jumpForce_(jumpForce), lastJump_(), jumpCd_(2000)
{
}

void FrogBigJumpLateral::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
}


void FrogBigJumpLateral::update()
{
	auto& RNG = sdlutils().rand();
	auto& vel = tr_->getVel();

	if (lastJump_ + jumpCd_ <= sdlutils().currRealTime() && !isJumping_) {
		isJumping_ = true;
		auto x = RNG.nextInt(-1000, 1000) / 100.0;
		/*	std::cout << x << std::endl;*/
		vel = new Vector2D(x, -jumpForce_);
		vel = vel.normalize() * jumpForce_;

	}

	if (isJumping_ && tr_->getPos().getY() >= sdlutils().height() - tr_->getHeight()) {
		isJumping_ = false;
		lastJump_ = sdlutils().currRealTime();
	};
}