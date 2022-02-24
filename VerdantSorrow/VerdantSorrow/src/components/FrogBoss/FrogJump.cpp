#include "FrogJump.h"

#include "../Transform.h"
#include "../FramedImage.h"
#include "../../ecs/Entity.h"

FrogJump::FrogJump(): tr_(), isJumping_(false), jumpForce_(), lastJump_(), jumpCd_(2000),fr_(nullptr)
{
}

FrogJump::FrogJump(float jumpForce): tr_(), isJumping_(false), jumpForce_(jumpForce), lastJump_(), jumpCd_(2000), fr_(nullptr)
{
}

void FrogJump::initComponent()
{
	fr_ = ent_->getComponent<FramedImage>();
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr||fr_!=nullptr);
}


void FrogJump::update()
{
	auto& RNG = sdlutils().rand();
	auto& vel = tr_->getVel();
	int direct=-1;
	if (lastJump_ + jumpCd_ <= sdlutils().currRealTime() && !isJumping_) {
		isJumping_ = true;
		auto x = RNG.nextInt(-1000, 1000) / 100.0;
	/*	std::cout << x << std::endl;*/
		vel = new Vector2D(x, -jumpForce_);	
		vel = vel.normalize() * jumpForce_;	
		if (x < 0)direct = -1;
		else direct = 1;
	}
	if (direct != dir) {
		//flipx 
		fr_->flipX(true);
	}

	if (isJumping_ && tr_->getPos().getY() >= sdlutils().height() - tr_->getHeight()) { 
		isJumping_ = false;
		lastJump_ = sdlutils().currRealTime();
	};
}
