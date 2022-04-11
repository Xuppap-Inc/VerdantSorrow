#include "ControlsScene.h"
#include "../components/Image.h"
#include "../ecs/Manager.h"
#include "../ecs/Entity.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../components/Transform.h"
#include "../sdlutils/Texture.h"

ControlsScene::ControlsScene():MenuScene(),changeSc_(false)
{
}

void ControlsScene::init()
{
	Scene::init();
	

	//background();//Dibuja el fondo

	int buttonWH = 50, imageW=800,imageH=300; //Ancho y alto del boton e imagen
	createButton(0, sdlutils().height()-buttonWH, buttonWH, buttonWH, "back");
	createImages(sdlutils().width() / 2-(imageW/2), sdlutils().height() / 2 - (imageH/ 2), imageW, imageH, "keyboardControls");

}
//
//void ControlsScene::background()
//{
//	Scene::background("fondoMenu");
//
//}

void ControlsScene::onButtonClicked(int index)
{
	changeSc_ = true;

	switch (index)
	{
	case 0: //Boton salida al menu principal
		sC().changeScene(SceneManager::Menu_);

		break;
	}
}

void ControlsScene::createImages(float x, float y, float w, float h, std::string controlsImage)
{
	auto newImage = mngr_->addEntity();
	auto tr = newImage->addComponent<Transform>(Vector2D(x, y), Vector2D(), w, h, 0.0f);
	newImage->addComponent<Image>(&sdlutils().images().at(controlsImage));

}

void ControlsScene::createText(std::string message)
{
	Texture text(sdlutils().renderer(), message,
		sdlutils().fonts().at("ARIAL24"), build_sdlcolor(0x444444ff));

	SDL_Rect dest = build_sdlrect(
		(sdlutils().width() - text.width()) / 2.0f, sdlutils().height()-100.0f, text.width(), text.height());

	text.render(dest);
}

void ControlsScene::update()
{
	handleInput();
	if(!changeSc_)
	{
		mngr_->update();
		mngr_->refresh();
		sdlutils().clearRenderer();
		mngr_->render();
		mngr_->debug();
		createText("Explicacion de prueba");
		sdlutils().presentRenderer();
	}
	
}
