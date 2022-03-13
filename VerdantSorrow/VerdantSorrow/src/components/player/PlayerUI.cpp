#include "PlayerUI.h"

#include "../../ecs/Entity.h"
#include "../../sdlutils/Texture.h"
#include "../../sdlutils/SDLUtils.h"

PlayerUI::PlayerUI(Texture* tex) : 
	tex_(tex), attrib_(nullptr),
	healthDetailTexture(&sdlutils().images().at("healthDetail"))
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
	int x = 100, y = 40, size = 50;
	for (int i = 0; i < attrib_->getLives(); i++) {
		tex_->render({ x,y,size,size });
		x += size + 20;
	}

	healthDetailTexture->render({ 10, 10, 450, 125 });
}

