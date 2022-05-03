#include "Settings.h"
#include "../components/Image.h"
#include "../ecs/Manager.h"
#include "../sdlutils/InputHandler.h"
Settings::Settings()
{

}
void Settings::init()
{
	changeSc_ = false;
	Scene::init();
	isChangingScene(changeSc_);
	generateAllButtons();
}

void Settings::update()
{
	handleInput(buttonPositions_, delay_, lastUpdate_, controllerIdex_, buttonNames_, buttonPoperties_, false);
	if (!changeSc_)
	{
		mngr_->update();
		mngr_->refresh();
		sdlutils().clearRenderer();
		mngr_->render();
		mngr_->debug();
		sdlutils().presentRenderer();
	}

}

void Settings::onButtonClicked(int index)
{

	isChangingScene(changeSc_);

	switch (index)
	{
	case 0: //Boton salida al menu principal
		if (sC().getPreviousScene() == SceneManager::Menu_) {
			sC().changeScene(SceneManager::Menu_);
			changeSc_ = true;
		}
		break;
	case 1: //Boton salida al menu principal
		sdlutils().setWindowSize(850, 480);
		init();
		break;
	case 2: //Boton salida al menu principal
		sdlutils().setWindowSize(256, 144);
		init();
		break;
	case 3: //Boton salida al menu principal
		sdlutils().setWindowSize(1920, 1080);
		
		init();
		break;
	case 4: //Boton salida al menu principal
		sdlutils().setWindowSize(1280, 720);
		init();
		break;
	}
}

void Settings::generateAllButtons()
{
	int buttonWH = 50;
	createButton(0, sdlutils().height() - buttonWH, buttonWH, buttonWH, buttonNames_[0], buttonPositions_, buttonPoperties_);
	//Variables que definen caracteristicas de los botones y numero de filas de botones en el menu
	int spacingX = 250; int buttonW = 200, buttonH = 80, iniX = 350, smallButtonWH = 80;
	
	////Bucle que dibuja la fila de botones
	int j = 0;
	for (int i = 1; i < 3; ++i)
	{
		createButton((sdlutils().width() / 2)  - buttonW + (j * spacingX), sdlutils().height() / 2 + buttonH * 0.6, buttonW, buttonH,
			buttonNames_[i], buttonPositions_, buttonPoperties_);
		++j;
	}

	j = 0;
	for (int i = 3; i < 5; ++i)
	{
		createButton((sdlutils().width() / 2) - buttonW  + (j * spacingX), sdlutils().height() / 2 - buttonH, buttonW, buttonH,
			buttonNames_[i], buttonPositions_, buttonPoperties_);
		++j;
	}
}

void Settings::background()
{
}


