#include "PlatformAtribsForHub.h"
#include "..\RectangleCollider.h"
#include "..\..\game\Game.h"


PlatformAtribsForHub::PlatformAtribsForHub(std::string sceneName) : sceneName_(sceneName)
{
	if (sceneName == "eye")scene = 3;
	else if (sceneName == "frog")scene = 1;
	else scene = 2;
}

void PlatformAtribsForHub::initComponent()
{
	auto rect = ent_->getComponent<RectangleCollider>();
	assert(rect != nullptr);
	int state = ((int)Game::instance()->state_);
	if (state == scene) {
		rect->setIsTrigger(true);
	}
}
