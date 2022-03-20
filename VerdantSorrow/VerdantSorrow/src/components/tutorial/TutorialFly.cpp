#include "TutorialFly.h"
#include "../../ecs/Entity.h"

TutorialFly::TutorialFly()
{
}

TutorialFly::~TutorialFly()
{

}

void TutorialFly::receiveHit()
{	
	ent_->setAlive(false);
}
