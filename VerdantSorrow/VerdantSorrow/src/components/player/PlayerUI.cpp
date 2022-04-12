#include "PlayerUI.h"

#include "../../ecs/Entity.h"
#include "../../ecs/Manager.h"
#include "../../sdlutils/Texture.h"
#include "../../sdlutils/SDLUtils.h"

PlayerUI::PlayerUI() : 
	heart(&sdlutils().images().at("heart")),
	heartBlack(&sdlutils().images().at("heartBlack")),
	healthDetailTexture(&sdlutils().images().at("healthDetail")),
	attrib_(nullptr)
{
}

PlayerUI::PlayerUI(bool hub) :
	heart(&sdlutils().imagesHub().at("heart")),
	heartBlack(&sdlutils().imagesHub().at("heartBlack")),
	healthDetailTexture(&sdlutils().imagesHub().at("healthDetail")),
	attrib_(nullptr)
{
}


PlayerUI::~PlayerUI()
{
}

void PlayerUI::initComponent()
{
	attrib_ = mngr_->getHandler(ecs::_PLAYER)->getComponent<PlayerAttributes>();
	assert(attrib_ != nullptr);
}

void PlayerUI::render()
{
	int x = 100, y = 40, size = 70;
	for (int i = 0; i < 3; i++) {

		SDL_Rect rect = { x, y, size, size };

		//escalado pantalla
		auto sW = mngr_->getWindowScaleWidth();
		auto sH = mngr_->getWindowScaleHeight();

		rect.x *= sW;
		rect.w *= sW;
		rect.y *= sH;
		rect.h *= sH;

		if (i < attrib_->getLives())
			heart->render(rect);
		else
			heartBlack->render(rect);

		x += size + 15;
	}

	SDL_Rect rect2 = { 10, 10, 450, 125 };

	//escalado pantalla
	auto sW = mngr_->getWindowScaleWidth();
	auto sH = mngr_->getWindowScaleHeight();

	rect2.x *= sW;
	rect2.w *= sW;
	rect2.y *= sH;
	rect2.h *= sH;

	healthDetailTexture->render(rect2);
}
