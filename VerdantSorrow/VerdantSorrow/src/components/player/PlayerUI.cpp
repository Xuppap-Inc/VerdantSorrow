#include "PlayerUI.h"

#include "../../ecs/Entity.h"
#include "../../sdlutils/Texture.h"
#include "../../sdlutils/SDLUtils.h"

PlayerUI::PlayerUI() : 
	heart(&sdlutils().images().at("heart")),
	heartBlack(&sdlutils().images().at("heartBlack")),
	healthDetailTexture(&sdlutils().images().at("healthDetail")),
	attrib_(nullptr)
{
}

PlayerUI::~PlayerUI()
{
}

void PlayerUI::initComponent()
{
	attrib_ = ent_->getComponent<PlayerAttributes>();
	assert(attrib_ != nullptr);
}

void PlayerUI::render()
{
	int x = 100, y = 40, size = 70;
	for (int i = 0; i < 3; i++) {

		if (i < attrib_->getLives())
			heart->render({ x,y,size,size });
		else
			heartBlack->render({ x,y,size,size });

		x += size + 15;
	}

	healthDetailTexture->render({ 10, 10, 450, 125 });
}
