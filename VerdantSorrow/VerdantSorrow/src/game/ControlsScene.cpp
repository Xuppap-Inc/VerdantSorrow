#include "ControlsScene.h"
#include "../components/Image.h"
#include "../ecs/Manager.h"
#include "../ecs/Entity.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../components/Transform.h"
#include "../sdlutils/Texture.h"


ControlsScene::ControlsScene() :BaseMenu(), delay_(250), lastUpdate_(0), controllerIdex_(-1)
{
}

void ControlsScene::init()
{
	changeSc_ = false;
	Scene::init();
	isChangingScene(changeSc_);


	int imageW = 800, imageH = 300; //Ancho y alto e imagen
	createImages(sdlutils().width() / 2 - (imageW / 2), sdlutils().height() / 2 - (imageH), imageW, imageH, "keyboardControls");
	generateAllButtons();
}
void ControlsScene::background()
{
	auto background_ = mngr_->addEntity();
	background_->addComponent<Transform>(Vector2D(0, 0), Vector2D(), sdlutils().width(), sdlutils().height(), 1.0f);
	background_->addComponent<Image>(&sdlutils().imagesHub().at("menuFondo"));
	background_->addToGroup(ecs::_BACKGROUND_1_GRP);
	
}

void ControlsScene::onButtonClicked(int index)
{
	changeSc_ = true;
	isChangingScene(changeSc_);

	switch (index)
	{
	case 0: //Boton salida al menu principal
		if (sC().getPreviousScene() == SceneManager::Menu_) {
			sC().changeScene(SceneManager::Menu_);
		}
		break;
	}
}

void ControlsScene::createImages(float x, float y, float w, float h, std::string controlsImage)
{
	auto newImage = mngr_->addEntity();
	auto tr = newImage->addComponent<Transform>(Vector2D(x, y), Vector2D(), w, h, 0.0f);
	newImage->addComponent<Image>(&sdlutils().imagesHub().at(controlsImage));

}

void ControlsScene::createText(float x, float y,std::string message)
{
	Texture text(sdlutils().renderer(), message,
		sdlutils().fontsHub().at("SOURCESERIF24"), build_sdlcolor(0x00000000));

	SDL_Rect rect = build_sdlrect(
		(x - text.width()) / 2.0f, y, text.width(), text.height());

	//escalado pantalla
	auto sW = mngr_->getWindowScaleWidth();
	auto sH = mngr_->getWindowScaleHeight();

	rect.x *= sW;
	rect.w *= sW;
	rect.y *= sH;
	rect.h *= sH;

	text.render(rect);
}

void ControlsScene::generateAllButtons()
{
	int buttonWH = 50;
	createButton(0, sdlutils().height() - buttonWH, buttonWH, buttonWH, buttonNames_[0], buttonPositions_, buttonPoperties_);
}

void ControlsScene::update()
{
	handleInput(buttonPositions_, delay_, lastUpdate_, controllerIdex_, buttonNames_, buttonPoperties_);
	if (!changeSc_)
	{
		mngr_->update();
		mngr_->refresh();
		sdlutils().clearRenderer();
		mngr_->render();
		mngr_->debug();
		createText(sdlutils().width(), sdlutils().height()-400,"Move  -> W,A,S,D");
		createText(sdlutils().width(), sdlutils().height()-350,"Jump -> W,SPACE");
		createText(sdlutils().width(), sdlutils().height()-300,"Interact with characters -> SPACE");
		createText(sdlutils().width(), sdlutils().height()-250,"Attack -> J,K,L");
		createText(sdlutils().width(), sdlutils().height()-200,"Roll -> LEFT SHIFT");
		sdlutils().presentRenderer();
	}

}
