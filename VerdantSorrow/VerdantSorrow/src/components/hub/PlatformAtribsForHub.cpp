#include "PlatformAtribsForHub.h"
#include "..\RectangleCollider.h"
#include "..\..\game\Game.h"
#include "../Image.h"
#include "../../sdlutils/SDLUtils.h"


PlatformAtribsForHub::PlatformAtribsForHub(std::string sceneName) : sceneName_(sceneName)
{
	if (sceneName == "eye")scene = 3;
	else if (sceneName == "frog")scene = 1;
	else scene = 2;


}

void PlatformAtribsForHub::initComponent()
{
	auto rect = ent_->getComponent<RectangleCollider>();
	auto image_ = ent_->getComponent<Image>();
	assert(rect != nullptr&&image_!=nullptr);
	int state = ((int)Game::instance()->state_);
	if (scene == state) {
		rect->setIsTrigger(true);
		image_->setTexture(&sdlutils().imagesHub().at(sceneName_ + "_open"));
	}
	else if (scene < state) {
		image_->setTexture(&sdlutils().imagesHub().at(sceneName_ + "_win"));
	}
	else {
		image_->setTexture(&sdlutils().imagesHub().at(sceneName_ + "_closed"));
	}

}
