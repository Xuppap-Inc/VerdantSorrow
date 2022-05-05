#include "CameraShake.h"
#include "Transform.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"

CameraShake::CameraShake(float shakeRad, int shakeDur, bool horizontal) : shakeSp_(0), tr_(nullptr), shakeRadius_(shakeRad), shakeDur_(shakeDur), iteration_(0), 
												shaking_(false), horizontal_(horizontal)
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
	auto& vel = tr_->getVel();
	if (shaking_) {
		if (horizontal_) {
			auto pos = tr_->getPos().getX();
			if (pos >= shakeRadius_) {
				iteration_ = 1;
				vel.setX(-vel.getX());
			}
			else if (pos <= 0.05 && pos >= -0.05) {
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
		else {
			auto pos = tr_->getPos().getY();
			if (pos >= shakeRadius_) {
				iteration_ = 1;
				vel.setY(-vel.getY());
			}
			else if (pos <= 0.05 && pos >= -0.05) {
				if (iteration_ == 1) {
					iteration_ = 2;
				}
				else if (iteration_ == 3) {
					shaking_ = false;
					vel.setY(0.0);
					tr_->getPos().setY(0);
				}
			}
			else if (pos <= -shakeRadius_) {
				iteration_ = 3;
				vel.setY(-vel.getY());
			}
		}
	}
}

void CameraShake::shake(float radius, float duration, bool horizontal)
{
	if (!shaking_) {
		shakeRadius_ = radius;
		shakeDur_ = duration;
		horizontal_ = horizontal;
		shaking_ = true;
		if (horizontal_)
			tr_->getVel().setX(shakeSp_);
		else
			tr_->getVel().setY(shakeSp_);
	}
}
