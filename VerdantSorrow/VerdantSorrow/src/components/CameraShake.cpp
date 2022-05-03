#include "CameraShake.h"
#include "Transform.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"

CameraShake::CameraShake(float shakeRad, int shakeDur) : shakeSp_(0), tr_(nullptr), shakeRadius_(shakeRad), shakeDur_(shakeDur), iteration_(0), shaking_(false)
{
}

void CameraShake::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	shakeSp_ = (shakeRadius_ * 4.0) / shakeDur_;
	assert(tr_ != nullptr);
}

void CameraShake::update()
{
	auto pos = tr_->getPos().getX();
	auto& vel = tr_->getVel();
	if (shaking_) {
		if (pos >= shakeRadius_) {
			iteration_ = 1;
			vel.setX(-vel.getX());
		}else if(pos <= 0.05 && pos>= -0.05){
			if (iteration_ == 1) {
				iteration_ = 2;
			}
			else if (iteration_ == 3) {
				shaking_ = false;
				vel.setX(0.0);
				tr_->getPos().setX(0);
			}
		}
		else if (pos <= -shakeRadius_) {
			iteration_ = 3;
			vel.setX(-vel.getX());
		}
	}
}

void CameraShake::shake(float radius, float duration)
{
	if (!shaking_) {
		shakeRadius_ = radius;
		shakeDur_ = duration;
		shaking_ = true;
		tr_->getVel().setX(shakeSp_);
	}
	
}
