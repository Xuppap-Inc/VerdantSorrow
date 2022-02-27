#include "FollowPlayer.h"
#include "../components/Transform.h"
#include "../ecs/Entity.h"

FollowPlayer::FollowPlayer(): 
	tr_(),
	playerTr_(),
	offsetX_(),
	offsetY_()
{
}
FollowPlayer::FollowPlayer(Transform *playerTransform_, float offsetX, float offsetY, Vector2D velocity, float followVelocity) :
	tr_(),
	playerTr_(playerTransform_),
	offsetX_(offsetX),
	offsetY_(offsetY),
	vel_(velocity),
	followVel_(followVelocity)
{
}
FollowPlayer::~FollowPlayer() {}

void FollowPlayer::initComponent()
{
	tr_ = ent_->getComponent<Transform>(); //Guarda el transform de la mosca
	assert(tr_ != nullptr);
	assert(playerTr_ != nullptr);
}

void FollowPlayer::update() 
{

	auto playerPos = playerTr_->getPos();
	auto playerVel = playerTr_->getVel();
	auto& flyPos = tr_->getPos();
	auto& flyVel = tr_->getVel();


	if (playerVel.magnitude() <= 0.01f)
	{
		flyVel = Vector2D(0.0f, 0.0f);
	}
	else
	{
		Vector2D distance = (playerPos - flyPos);
		auto velX = distance.normalize().getX();
		if (abs(distance.getX()) <= offsetX_) 
		{
			velX = 0.0f;
		}

		flyVel = Vector2D((velX * followVel_), distance.getY()+ offsetY_);
		
	}

	
}