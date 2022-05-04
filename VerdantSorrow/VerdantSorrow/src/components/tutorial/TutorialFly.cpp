#include "TutorialFly.h"
#include "../../ecs/Entity.h"
#include "../../ecs/Manager.h"

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
